#include <gtest/gtest.h>
#include <curl/curl.h>
#include <string>
#include <memory>

/**
 * libcurl Tests
 *
 * These tests verify libcurl functionality for making HTTP requests.
 */

// Test fixture for Curl tests
class CurlTests : public ::testing::Test {
protected:
    // Callback function for libcurl
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
};

// Test libcurl initialization
TEST_F(CurlTests, CurlInitialization)
{
    CURL* curl = curl_easy_init();
    EXPECT_NE(curl, nullptr) << "CURL initialization should succeed";

    if (curl)
    {
        curl_easy_cleanup(curl);
    }
}

// Test global libcurl initialization
TEST_F(CurlTests, CurlGlobalInit)
{
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    EXPECT_EQ(CURLE_OK, res) << "Global CURL initialization should succeed";

    curl_global_cleanup();
}

// Test simple HTTP GET request
TEST_F(CurlTests, HttpGetRequest)
{
    CURL* curl = curl_easy_init();
    ASSERT_NE(curl, nullptr) << "CURL initialization should succeed";

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
        EXPECT_EQ(CURLE_OK, res) << "HTTP GET request should succeed";

        // Check that we received some data
        EXPECT_GT(readBuffer.length(), 0u) << "Response should contain data";

        // Check HTTP response code
        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        EXPECT_EQ(200, httpCode) << "HTTP status code should be 200 OK";

        curl_easy_cleanup(curl);
    }
}

// Test HTTPS request
TEST_F(CurlTests, HttpsGetRequest)
{
    CURL* curl = curl_easy_init();
    ASSERT_NE(curl, nullptr) << "CURL initialization should succeed";

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
        EXPECT_EQ(CURLE_OK, res) << "HTTPS GET request should succeed";

        // Verify we got data
        EXPECT_GT(readBuffer.length(), 0u) << "Response should contain data";

        curl_easy_cleanup(curl);
    }
}

// Test redirect following
TEST_F(CurlTests, FollowRedirects)
{
    CURL* curl = curl_easy_init();
    ASSERT_NE(curl, nullptr) << "CURL initialization should succeed";

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
        EXPECT_EQ(CURLE_OK, res) << "Request with redirect following should succeed";

        curl_easy_cleanup(curl);
    }
}

// Test timeout handling
TEST_F(CurlTests, TimeoutHandling)
{
    CURL* curl = curl_easy_init();
    ASSERT_NE(curl, nullptr) << "CURL initialization should succeed";

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
        EXPECT_TRUE(res == CURLE_OPERATION_TIMEDOUT || res == CURLE_COULDNT_CONNECT)
            << "Request to non-routable IP should timeout or fail to connect";

        curl_easy_cleanup(curl);
    }
}

// Test user agent setting
TEST_F(CurlTests, UserAgentSetting)
{
    CURL* curl = curl_easy_init();
    ASSERT_NE(curl, nullptr) << "CURL initialization should succeed";

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
        EXPECT_EQ(CURLE_OK, res) << "Request with custom user agent should succeed";

        curl_easy_cleanup(curl);
    }
}

// Note about running these tests:
// These tests make real HTTP requests to example.com
// Ensure you have:
// 1. Internet connectivity
// 2. libcurl properly linked with OpenSSL for HTTPS
// 3. Firewall allows outbound HTTP/HTTPS connections
//
// Run with: ./cppgorating_tests --gtest_filter=CurlTests.*
