/*
 * The main file of GeoPictureSearch. It will start the service.
 * Date: 2015-05-16
 * Author: galoisrain 
 */

#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "glog/logging.h"

#include "flag.h"
#include "GeoServer.h"

namespace GeoPicture {

    DEFINE_int32(server_port, 6060, "ms server port");
    DEFINE_int32(server_thread_num, 10, "thread pool server handler thread num");
}


void handle_signal(int sig) {
    LOG(INFO) << "kill over!" << endl;
    exit(0);
}

int main() {
    ::google::ParseCommandLineFlags(&argc, &argv, false);
    ::google::SetUsageMessage("Geo picture search server");

    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGPIPE, SIG_IGN);

    GeoServer geo_server;
    if (0 != geo_server.init()) {
        LOG(WARNING) << "init uid server wrong!" << endl;
        return 0;
    }

    if (0 != geo_server.run()) {
        LOG(WARNING) << "run uid server error!" << endl;
        return 0;
    }

    return 0;
}
