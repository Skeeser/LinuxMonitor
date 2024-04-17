#include <zlib>

namespace network
{
    class Utils
    {
    public:
        // 计算给定数据的无符号32位校验和
        static int32_t checkSum(const void *buf, int len)
        {
            return static_cast<int32_t>(::adler32(1, static_cast<const Bytef *>(buf), len));
        }

        // 检验校验和
        static bool validateChecksum(const char *buf, int len, int kChecksumLen)
        {
            // check sum
            int32_t expectedCheckSum = asInt32(buf + len - kChecksumLen);
            int32_t checkSum = checkSum(buf, len - kChecksumLen);
            return expectedCheckSum == checkSum;
        }

        // buf转换成int32
        static int32_t asInt32(const char *buf)
        {
            int32_t be32 = 0;
            ::memcpy(&be32, buf, sizeof(be32));
            // 网络顺序(大端)转换成主机顺序
            return sockets::networkToHost32(be32);
        }
    };

}