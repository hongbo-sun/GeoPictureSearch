/*
 * Geo Server Implementation.
 * Date: 2015-05-16
 * Author: galoisrain 
 */
#ifndef __GEO_SERVER_H_
#define __GEO_SERVER_H_

namespace apache { namespace thrift {namespace server {
    class TThreadPoolServer;
}}}

namespace GeoPicture {
    class GeoServer {
        public:
            GeoServer() {
                server_ = NULL;
            }
            ~UidServer() {
                if (NULL != server_) {
                    delete server_;
                    server_ = NULL;
                }
            }

            int init();
            int run();
            void stop();
        private:
            apache::thrift::server::TThreadPoolServer *server_;
    };
}

#endif
