#include <curl/curl.h>
#include <curl/multi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/prctl.h>

fff
/* mutil 句柄的最大连接数 */
#define  MAX_CONNECT_HANDLE_NUMS  (15)
CURLM * m_MultiHandle = NULL;


typedef struct github_api_response_data
{
    char* data;
    int data_size;
} github_api_response_data_t;

/* 客制化的私有数据结构体 */
typedef struct github_api_multicurl_privdata
{
    struct curl_slist *headers;
	 struct github_api_response_data res_data;
	 struct github_api_response_data res_header;
}github_api_multicurl_privdata_t;

static size_t github_api_response_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
	//mem->data_size + realsize + 1
    github_api_response_data_t * mem = (github_api_response_data_t*)data;
    mem->data = (char *)realloc(mem->data, mem->data_size + realsize + 1);
    if (mem->data == NULL)
    {
        return -1;
    }
    
    memcpy(&(mem->data[mem->data_size]), ptr, realsize);
    mem->data_size += realsize;
    mem->data[mem->data_size] = '\0';
    return realsize;
}


void MultiperformFunc()
{
    int m_IsStillRuning = 1;
    int msgs_left = 0;
	int still_running = 0;
	int loop = m_MultiHandle;
    while(loop--)
    {
		CURLMcode mcode = curl_multi_perform(m_MultiHandle, &still_running);
		while(mcode == CURLM_CALL_MULTI_PERFORM)
		{
			mcode = curl_multi_perform(m_MultiHandle, &still_running);
		}

        CURLMsg *msg = NULL;
        int numfds = 0;

        while((msg = curl_multi_info_read(m_MultiHandle, &msgs_left)))
        {
        	printf("\r\nfffffffffffffffffffffffffff\r\n");
            if(msg->msg == CURLMSG_DONE)
            {
            	printf("CURLMSG_DONE\n");
                CustomPrivData* PrivData = NULL;
                CURL *e = msg->easy_handle;
                curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &PrivData);
                if(PrivData)
                {
                	/* 处理每个easyhandle的私有数据 */
                    if(PrivData->wfd)
                        fclose(PrivData->wfd);
                    if(PrivData->headers)
                        curl_slist_free_all(PrivData->headers);
					if(PrivData->res_header.data)
						printf("%s",PrivData->res_header.data);
                    free(PrivData);
                    PrivData = NULL;
                }
                
                /*当一个easy handler传输完成，此easy handler仍然仍然停留在multi stack中,
                调用curl_multi_remove_handle将其从multi stack中移除,然后调用curl_easy_cleanup将其关闭*/
                curl_multi_remove_handle(m_MultiHandle, e);
                curl_easy_cleanup(e);
            }
            else
            {
            }
        }

        usleep(30000);
		
		printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
    }
}


int main(int argc, char* argv[])
{
	curl_global_init(CURL_GLOBAL_ALL);
		
	m_MultiHandle  = curl_multi_init();
	curl_multi_setopt(m_MultiHandle, CURLMOPT_MAXCONNECTS, MAX_CONNECT_HANDLE_NUMS);

	CURLSH* m_ShareHandle = curl_share_init();
	if(m_ShareHandle)
	{
		/* 设置DNS共享，不然每个easyhandle执行时都要解析一遍DNS */
		curl_share_setopt(m_ShareHandle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
	}
	

	CURL*   EasyCurlHandle = curl_easy_init();

    if(EasyCurlHandle)
    {

        struct curl_slist *headers = NULL;
        char Auth_header[256]={0};
        snprintf(Auth_header, 256, "token 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");
        headers = curl_slist_append(headers, Auth_header);

        github_api_multicurl_privdata_t* PrivData = (github_api_multicurl_privdata_t *)calloc(sizeof(github_api_multicurl_privdata_t), 1);

        //headers = curl_slist_append(headers, "User-agent:Mozilla/5.0(Windows NT 6.1;Win64; x64)");
        //headers = curl_slist_append(headers, "Accept-Encoding:gzip,deflate");
        //headers = curl_slist_append(headers, "Accept-Language:zh-CN,zh;q=0.9");
		curl_easy_setopt(EasyCurlHandle, CURLOPT_NOSIGNAL,1);
		curl_easy_setopt(EasyCurlHandle, CURLOPT_CONNECTTIMEOUT,10);
		curl_easy_setopt(EasyCurlHandle, CURLOPT_TIMEOUT,10);


        curl_easy_setopt(EasyCurlHandle, CURLOPT_HTTPHEADER, headers);
		
        snprintf(Auth_header, 256, "User-agent: PostmanRuntime/7.26.10");
        headers = curl_slist_append(headers, Auth_header);
        curl_easy_setopt(EasyCurlHandle, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(EasyCurlHandle, CURLOPT_WRITEFUNCTION, &github_api_response_callback);
        curl_easy_setopt(EasyCurlHandle, CURLOPT_WRITEDATA, &PrivData->res_data);
        
        curl_easy_setopt(EasyCurlHandle, CURLOPT_HEADERFUNCTION, &github_api_response_callback);
        curl_easy_setopt(EasyCurlHandle, CURLOPT_HEADERDATA, &PrivData->res_header);
		
        PrivData->headers = headers;
	
        /* 设置DNS cache的超时时间为120s */
        curl_easy_setopt(EasyCurlHandle, CURLOPT_DNS_CACHE_TIMEOUT, 60*2);
        curl_easy_setopt(EasyCurlHandle, CURLOPT_SHARE, m_ShareHandle);

        curl_easy_setopt(EasyCurlHandle, CURLOPT_URL, "https://api.github.com/repos/GHTesterOrg-A/PubRepo/commits");         /* URL地址设置 */
        
        
		/* 设置私有数据用于curl_multi_perform执行后区分不同easy句柄 */
        curl_easy_setopt(EasyCurlHandle, CURLOPT_PRIVATE, PrivData);

       /* 添加到multi句柄 */
        curl_multi_add_handle(m_MultiHandle, EasyCurlHandle);

        /* headers、mime不能在此处释放 ,否则调用multi接口时奔溃*/
        //curl_slist_free_all(headers);
        //curl_mime_free(mime);
        
		
    }

	
	MultiperformFunc();

}


