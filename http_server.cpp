#include <memory>
#include <cstdint>
#include <iostream>
#include <evhttp.h>
#include <string>
#include "dictionary.h"
using namespace std;
using namespace chrono;

#define SRV_ADDR "127.0.0.1"
#define PORT 8000
#define URI_PREFIX "/api/v1/similar?word="
#define URI_PREFIX_LEN strlen(URI_PREFIX)
#define STATS_URI "/api/v1/stats"

Dictionary dict;

/**************************************************************
* @brief   callback function in order to respond to the       *
*          incoming requests (passed to evhttp_set_gencb).    *
* @param   The event request from client.                     *
* @return                                                     *
**************************************************************/
void (*OnReq)(evhttp_request *req, void *) = [] (evhttp_request *req, void *)
{
    string reply;
    string uri(evhttp_request_get_uri(req));
    auto *OutBuf = evhttp_request_get_output_buffer(req);
    if(!OutBuf) {
        cerr << "Failed to get the returned output buffer." << endl;
        return;
    }
    /* This is a state query */
    if (uri == STATS_URI) {
        reply = dict.get_dictionary_state();
    } else {
        if (uri.rfind(URI_PREFIX) != 0) {
            return;
        }
        /* This is word permutations queary */
        reply = dict.get_permutations(uri.substr(URI_PREFIX_LEN));
    }
    /* Send reply back to the client */
    evbuffer_add_printf(OutBuf, "%s", reply.c_str());
    evhttp_send_reply(req, HTTP_OK, "", OutBuf);
};

int main()
{
    /* Init dictionary DB. Error will be printted in class funtion */
    if (!dict.init_dictionary_db()) {
    	return -1;
    }
    /* Initialize a global object of the library */
    if (!event_init()) {
        cerr << "Failed to init libevent" << endl;
        return -1;
    }
    /* Create http server */
    unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SRV_ADDR, PORT), &evhttp_free);
    if (!Server) {
        cerr << "Failed to init http server" << endl;
        return -1;
    }
    /* Start the loop of event processing */ 
    evhttp_set_gencb(Server.get(), OnReq, nullptr);
    if (event_dispatch() == -1) {
        cerr << "Failed to run event loop" << endl;
        return -1;
    }
    return 0;
}
