#ifndef __MUDUO_NET_BEAST_WRITE_BUF_LAMBDA_H__
#define __MUDUO_NET_BEAST_WRITE_BUF_LAMBDA_H__

#include <boost/beast.hpp>
#include "muduo/net/Buffer.h"

namespace zophyros
{

template<typename Serializer>
class WriteBufLambda
{
public:
    WriteBufLambda(muduo::net::Buffer& buf, Serializer& sr)
        : buffer_m(buf)
        , sr_m(sr)
    {
    }

    template<typename ConstBufferSequence>
    void 
    operator()(boost::beast::error_code& ec, 
                ConstBufferSequence const& buffers) const
    {
        ec = {};
        std::size_t bytesTransfered = 0;
        for(auto b : boost::beast::buffers_range_ref(buffers))
        {
            buffer_m.append(b.data(), b.size());
            bytesTransfered += b.size();
        }
        sr_m.consume(bytesTransfered);
    }

private:
    muduo::net::Buffer& buffer_m;
    Serializer& sr_m;
};


}

#endif