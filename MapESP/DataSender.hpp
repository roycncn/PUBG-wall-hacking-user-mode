#pragma once
#include <curl/curl.h>
#include <iostream>


class DataSender
{
public:

	// Constructor of this wrapper. Also initializes the CURL.
	DataSender()
	{
		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();
	}

	// destructor of this wrapper
	~DataSender()
	{
		curl_easy_cleanup(curl);
	}

	bool isReady()
	{
		if (curl) return true;
		return false;
	}

	static size_t write_data(void* w_buffer, size_t w_size, size_t w_nmemb, void* w_userp)
	{
		return w_size * w_nmemb;
	}

	int sendData(std::string& URL,std::string& dataPack)
	{
		try
		{
			struct curl_slist *headers = NULL;

			headers = curl_slist_append(headers, "Content-Type: application/json");

			curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_URL, URL.data());
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
			// curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 30L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataPack.data());
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

			curl_easy_perform(curl);

			curl_slist_free_all(headers);
			curl_easy_reset(curl);


		}
		catch (std::exception& e)
		{
			return -1;
		}
	}

private:
	CURL* curl;			
};


