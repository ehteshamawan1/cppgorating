#include <UnitTest++/UnitTest++.h>
#include <curl/curl.h>
#include <string>
#include <memory>

/**
 * libcurl Tests
 *
 * These tests verify libcurl functionality for making HTTP requests.
 */

SUITE(CurlTests)
{
    // Callback function for libcurl
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    // Test libcurl initialization
    TEST(CurlInitialization)
    {
        CURL* curl = curl_easy_init();
        CHECK(curl != nullptr);

        if (curl)
        {
            curl_easy_cleanup(curl);
        }
    }

    // Test global libcurl initialization
    TEST(CurlGlobalInit)
    {
        CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
        CHECK_EQUAL(CURLE_OK, res);

        curl_global_cleanup();
    }

    // Test simple HTTP GET request
    TEST(HttpGetRequest)
    {
        CURL* curl = curl_easy_init();
        CHECK(curl != nullptr);

        if (curl)
        {
            std::string readBuffer;

            // Use a reliable test URL (example.com is specifically for testing)
            curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

            CURLcode res = curl_easy_perform(curl);

            // Check that request completed without errors
            CHECK_EQUAL(CURLE_OK, res);

            // Check that we received some data
            CHECK(readBuffer.length() > 0);

            // Check HTTP response code
            long httpCode = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
            CHECK_EQUAL(200, httpCode);

            curl_easy_cleanup(curl);
        }
    }

    // Test HTTPS request
    TEST(HttpsGetRequest)
    {
        CURL* curl = curl_easy_init();
        CHECK(curl != nullptr);

        if (curl)
        {
            std::string readBuffer;

            // Test HTTPS (requires OpenSSL)
            curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

            CURLcode res = curl_easy_perform(curl);

            // HTTPS should work with proper OpenSSL configuration
            CHECK_EQUAL(CURLE_OK, res);

            // Verify we got data
            CHECK(readBuffer.length() > 0);

            curl_easy_cleanup(curl);
        }
    }

    // Test redirect following
    TEST(FollowRedirects)
    {
        CURL* curl = curl_easy_init();
        CHECK(curl != nullptr);

        if (curl)
        {
            std::string readBuffer;

            curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

            CURLcode res = curl_easy_perform(curl);
            CHECK_EQUAL(CURLE_OK, res);

            curl_easy_cleanup(curl);
        }
    }

    // Test timeout handling
    TEST(TimeoutHandling)
    {
        CURL* curl = curl_easy_init();
        CHECK(curl != nullptr);

        if (curl)
        {
            std::string readBuffer;

            // Use a URL that will timeout (non-routable IP)
            curl_easy_setopt(curl, CURLOPT_URL, "http://10.255.255.1");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 2L);  // 2 second timeout
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2L);

            CURLcode res = curl_easy_perform(curl);

            // Should timeout
            CHECK(res == CURLE_OPERATION_TIMEDOUT || res == CURLE_COULDNT_CONNECT);

            curl_easy_cleanup(curl);
        }
    }

    // Test user agent setting
    TEST(UserAgentSetting)
    {
        CURL* curl = curl_easy_init();
        CHECK(curl != nullptr);

        if (curl)
        {
            std::string readBuffer;
            const char* userAgent = "cppgorating-test/1.0";

            curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

            CURLcode res = curl_easy_perform(curl);
            CHECK_EQUAL(CURLE_OK, res);

            curl_easy_cleanup(curl);
        }
    }
}

// Note about running these tests:
// These tests make real HTTP requests to example.com
// Ensure you have:
// 1. Internet connectivity
// 2. libcurl properly linked with OpenSSL for HTTPS
// 3. Firewall allows outbound HTTP/HTTPS connections
//
// Run with: ./cppgorating_tests -suite CurlTests
