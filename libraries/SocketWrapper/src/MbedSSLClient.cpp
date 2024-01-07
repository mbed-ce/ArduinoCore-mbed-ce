#include "MbedSSLClient.h"

#ifdef MBEDTLS_SSL_CLI_C

arduino::MbedSSLClient::MbedSSLClient(unsigned long  timeout): MbedClient(timeout), _disableSNI{false} {
  onBeforeConnect(mbed::callback(this, &MbedSSLClient::setRootCA));
}

arduino::MbedSSLClient::MbedSSLClient(): _disableSNI{false} {
  onBeforeConnect(mbed::callback(this, &MbedSSLClient::setRootCA));
};

#endif
