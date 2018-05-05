#include <iostream>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <string.h>
#include <array>
#include <cstdlib>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdlib.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;

std::string execP(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
			result += buffer.data();
	}
	return result;
}

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
    size_t oldLength = s->size();
    try
    {
        s->resize(oldLength + newLength);
    }
    catch(std::bad_alloc &e)
    {
        //handle memory problem
        return 0;
    }

    std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
    return size*nmemb;
}

int check_url(char *url)
{
    CURL *curl;
    CURLcode response;
    curl = curl_easy_init();

    std::string s="";
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* don't write output to stdout */
        //curl_easy_setopt(curl, CURLOPT_NOBODY, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        /* Perform the request */
        response = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    //cout << url << endl;
    //cout << s.size() << endl;
    //cout << s<<endl;
    //cout << response << endl;
    return ((response == CURLE_OK || response == CURLE_OPERATION_TIMEDOUT ) && s.size()>0) ? 1 : 0;
}


// main () is where program execution begins
int main(int argc, char** argv)
{
    while(true)
    {
        cout << "Checking again"<<endl;
        // ifstream file;
	    // file.open("ffmpeg.txt");
	    // char ffmpeg[2000];
        // if (file.is_open()) {
        //     while (!file.eof()) {
        //         file >> ffmpeg;
        //     }
        // }
        // file.close();
        //
        int check = check_url(argv[1]);
        
        string fid = execP("pidof ffmpeg");
        if(fid.size()>0)
        {
            fid = fid.substr(fid.find(" ")+1);
        }
        cout <<fid<<endl;

        if(check==0){
            cout << "Live stream is down"<<endl;

            if(fid.size()>0){
                string cmd = "kill -KILL " + fid;
                cout << "Killing FFMPEG" << cmd << endl;
                execP(cmd.c_str());
            }
        }
        else
        {
            cout << "Live stream is up"<<endl;
            if(fid.size()==0)
            {
                system("./runffmpeg &");
                cout << "FFMPEG Runned."<<endl;
            }
        }
        std::this_thread::sleep_for (std::chrono::seconds(10));
    }
    
  return 0;
}