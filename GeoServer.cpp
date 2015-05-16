#include <pthread.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/server/TThreadedServer.h>

#include "glog/logging.h"
#include "flag.h"

#include "GeoServer.h"
#include "GeoServerHandler.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace GeoPicture;

using boost::shared_ptr;

int GeoServer::init() {
    //初始化日志
    FLAGS_logbuflevel = -1;
    google::InitGoogleLogging("geo_server");
    return 0;
}

int GeoServer::run() {
    boost::shared_ptr<GeoServerHandler> handler(new GeoServerHandler());

    // transport
    boost::shared_ptr<TProcessor> processor(new GeoServerProcessor(handler));
    boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(FLAGS_server_port));
    boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());

    // protocol
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    // thread pool
    shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(FLAGS_server_thread_num);
    boost::shared_ptr<PosixThreadFactory> threadFactory =
        boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();

    TThreadPoolServer server(processor,
                             serverTransport,
                             transportFactory,
                             protocolFactory,
                             threadManager);

    LOG(INFO) << "Starting the server..." << endl;
    server.serve();
    LOG(INFO) << "Done." << endl;
    return 0;
}

void GeoServer::stop() {
    if (server_) {
        server_->stop();
    }
}

