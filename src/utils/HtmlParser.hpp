#pragma once

#include <gumbo.h>
#include <string>
#include <vector>
#include <memory>

/**
 * HtmlParser - Utility class for parsing HTML using Gumbo parser
 *
 * This class provides methods to extract common elements from HTML:
 * - Page title
 * - Meta tags (description, keywords, etc.)
 * - Links (anchor tags)
 * - Headings
 */
class HtmlParser
{
public:
    struct ParsedData
    {
        std::string title;
        std::string metaDescription;
        std::string metaKeywords;
        std::vector<std::string> links;
        std::vector<std::string> headings;
    };

    /**
     * Parse HTML string and extract common elements
     * @param html Raw HTML content
     * @return ParsedData structure with extracted information
     */
    static ParsedData parse(const std::string& html)
    {
        ParsedData data;

        if (html.empty())
        {
            return data;
        }

        // Parse HTML with Gumbo
        GumboOutput* output = gumbo_parse(html.c_str());
        if (!output)
        {
            return data;
        }

        // Extract title
        data.title = extractTitle(output->root);

        // Extract meta tags
        extractMetaTags(output->root, data);

        // Extract links
        data.links = extractLinks(output->root);

        // Extract headings
        data.headings = extractHeadings(output->root);

        // Clean up
        gumbo_destroy_output(&kGumboDefaultOptions, output);

        return data;
    }

private:
    /**
     * Extract text content from a node
     */
    static std::string extractText(GumboNode* node)
    {
        if (node->type == GUMBO_NODE_TEXT)
        {
            return std::string(node->v.text.text);
        }
        else if (node->type == GUMBO_NODE_ELEMENT)
        {
            std::string text;
            GumboVector* children = &node->v.element.children;
            for (unsigned int i = 0; i < children->length; ++i)
            {
                text += extractText(static_cast<GumboNode*>(children->data[i]));
            }
            return text;
        }
        return "";
    }

    /**
     * Extract title tag content
     */
    static std::string extractTitle(GumboNode* node)
    {
        if (node->type != GUMBO_NODE_ELEMENT)
        {
            return "";
        }

        if (node->v.element.tag == GUMBO_TAG_TITLE)
        {
            return extractText(node);
        }

        // Recursively search children
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
        {
            std::string title = extractTitle(static_cast<GumboNode*>(children->data[i]));
            if (!title.empty())
            {
                return title;
            }
        }

        return "";
    }

    /**
     * Extract meta tags (description, keywords)
     */
    static void extractMetaTags(GumboNode* node, ParsedData& data)
    {
        if (node->type != GUMBO_NODE_ELEMENT)
        {
            return;
        }

        if (node->v.element.tag == GUMBO_TAG_META)
        {
            GumboAttribute* name = gumbo_get_attribute(&node->v.element.attributes, "name");
            GumboAttribute* content = gumbo_get_attribute(&node->v.element.attributes, "content");

            if (name && content)
            {
                std::string nameValue = name->value;
                if (nameValue == "description")
                {
                    data.metaDescription = content->value;
                }
                else if (nameValue == "keywords")
                {
                    data.metaKeywords = content->value;
                }
            }
        }

        // Recursively search children
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
        {
            extractMetaTags(static_cast<GumboNode*>(children->data[i]), data);
        }
    }

    /**
     * Extract all links (anchor tags with href attribute)
     */
    static std::vector<std::string> extractLinks(GumboNode* node)
    {
        std::vector<std::string> links;

        if (node->type != GUMBO_NODE_ELEMENT)
        {
            return links;
        }

        if (node->v.element.tag == GUMBO_TAG_A)
        {
            GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
            if (href)
            {
                links.push_back(href->value);
            }
        }

        // Recursively search children
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
        {
            std::vector<std::string> childLinks = extractLinks(static_cast<GumboNode*>(children->data[i]));
            links.insert(links.end(), childLinks.begin(), childLinks.end());
        }

        return links;
    }

    /**
     * Extract all headings (h1, h2, h3, etc.)
     */
    static std::vector<std::string> extractHeadings(GumboNode* node)
    {
        std::vector<std::string> headings;

        if (node->type != GUMBO_NODE_ELEMENT)
        {
            return headings;
        }

        // Check if this is a heading tag
        GumboTag tag = node->v.element.tag;
        if (tag == GUMBO_TAG_H1 || tag == GUMBO_TAG_H2 || tag == GUMBO_TAG_H3 ||
            tag == GUMBO_TAG_H4 || tag == GUMBO_TAG_H5 || tag == GUMBO_TAG_H6)
        {
            headings.push_back(extractText(node));
        }

        // Recursively search children
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
        {
            std::vector<std::string> childHeadings = extractHeadings(static_cast<GumboNode*>(children->data[i]));
            headings.insert(headings.end(), childHeadings.begin(), childHeadings.end());
        }

        return headings;
    }
};
