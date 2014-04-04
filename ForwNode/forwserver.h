#ifndef FORWSERVER_H
#define FORWSERVER_H

namespace Dosee
{
namespace ForwNode
{

typedef struct _tag_write_req_t{
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;

class ForwServer
{
public:
    ForwServer();
};

}}
#endif // FORWSERVER_H
