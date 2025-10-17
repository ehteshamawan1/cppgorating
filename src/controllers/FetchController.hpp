#pragma once

#include <drogon/HttpController.h>
#include <curl/curl.h>
#include <string>
#include <memory>
#include "../utils/HtmlParser.hpp"

using namespace drogon;

/**
 * FetchController - Handles HTTP requests to external URLs
 *
 * This controller is added in Phase 4 to demonstrate libcurl integration.
 * Endpoints:
 * - GET /fetch?url=<url> - Fetches content from external URL
 * - GET /fetch?url=<url>&parse=true - Fetches and parses HTML (Phase 4)
 */
class FetchController : public drogon::HttpController<FetchController>
{
public:
    METHOD_LIST_BEGIN
    // Register GET handler for "/fetch"
    ADD_METHOD_TO(FetchController::fetch, "/fetch", Get);
    METHOD_LIST_END

    /**
     * Callback function for libcurl to write received data
     */
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    /**
     * GET /fetch handler
     * Fetches content from external URL using libcurl
     *
     * Query parameters:
     * - url: URL to fetch (required)
     * - parse: Set to "true" to parse HTML (optional, Phase 4)
     */
    void fetch(const HttpRequestPtr& req,
               std::function<void(const HttpResponsePtr&)>&& callback)
    {
        // Get URL parameter
        std::string url = req->getParameter("url");
        if (url.empty())
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k400BadRequest);
            resp->setBody("Missing 'url' parameter");
            callback(resp);
            return;
        }

        // Check if parsing is requested (Phase 4 feature)
        std::string parse = req->getParameter("parse");
        bool shouldParse = (parse == "true" || parse == "1");

        // Initialize libcurl
        CURL* curl = curl_easy_init();
        if (!curl)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k500InternalServerError);
            resp->setBody("Failed to initialize libcurl");
            callback(resp);
            return;
        }

        // Buffer to store fetched content
        std::string readBuffer;

        // Configure libcurl
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  // Follow redirects
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);         // 30 second timeout
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "cppgorating/1.0");

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
        {
            std::string errorMsg = "libcurl error: ";
            errorMsg += curl_easy_strerror(res);

            curl_easy_cleanup(curl);

            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k500InternalServerError);
            resp->setBody(errorMsg);
            callback(resp);
            return;
        }

        // Get HTTP response code
        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        // Cleanup libcurl
        curl_easy_cleanup(curl);

        // Prepare response
        Json::Value jsonResponse;
        jsonResponse["url"] = url;
        jsonResponse["status_code"] = (int)httpCode;
        jsonResponse["html"] = readBuffer;

        // If parsing is requested, parse HTML with Gumbo
        if (shouldParse && !readBuffer.empty())
        {
            try
            {
                HtmlParser::ParsedData parsedData = HtmlParser::parse(readBuffer);

                Json::Value parsed;
                parsed["title"] = parsedData.title;
                parsed["meta_description"] = parsedData.metaDescription;
                parsed["meta_keywords"] = parsedData.metaKeywords;

                // Add links array
                Json::Value linksArray(Json::arrayValue);
                for (const auto& link : parsedData.links)
                {
                    linksArray.append(link);
                }
                parsed["links"] = linksArray;

                // Add headings array
                Json::Value headingsArray(Json::arrayValue);
                for (const auto& heading : parsedData.headings)
                {
                    headingsArray.append(heading);
                }
                parsed["headings"] = headingsArray;

                jsonResponse["parsed"] = parsed;
            }
            catch (const std::exception& e)
            {
                Json::Value parsed;
                parsed["error"] = std::string("HTML parsing failed: ") + e.what();
                jsonResponse["parsed"] = parsed;
            }
        }

        // Return JSON response
        auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
        resp->setStatusCode(k200OK);
        callback(resp);
    }
};
