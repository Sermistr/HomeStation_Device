#pragma once


namespace WiFiConfigTemplate
{
    // Wi-Fi configuration
    constexpr char OFFICE_SSID[] = "YourOfficeSSID"; // Replace with your Wi-Fi SSID
    constexpr char OFFICE_PASSWORD[] = "YourOfficePassword"; // Replace with your Wi-Fi password

    constexpr char HOME_SSID[] = "YourHomeSSID"; // Replace with your Wi-Fi SSID
    constexpr char HOME_PASSWORD[] = "YourHomePassword"; // Replace with your Wi-Fi password

    // Telegram bot configuration
    constexpr char BOT_TOKEN[] = "YourBotToken";  // Replace with your Telegram bot token
    constexpr char ADMIN_CHAT_ID[] = "YourAdminChatID"; // Replace with the Telegram chat ID of the admin user

    // Groq (LLM) configuration - OpenAI-compatible API
    constexpr char GROQ_API_KEY[] = "YourGroqAPIKey";       // Get a key at https://console.groq.com/keys
    constexpr char GROQ_MODEL[]   = "llama-3.1-8b-instant"; // Groq model (llama-3.1-8b-instant, llama-3.3-70b-versatile, ...)
}
