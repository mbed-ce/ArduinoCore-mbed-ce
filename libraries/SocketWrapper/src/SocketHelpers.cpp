#include "SocketHelpers.h"
#include "utility/http_request.h"
#include "utility/https_request.h"


namespace arduino
{
uint8_t* MbedSocketClass::macAddress(uint8_t* mac) {
  const char* mac_str = getNetwork()->get_mac_address();
  for (int b = 0; b < 6; b++) {
    uint32_t tmp;
    sscanf(&mac_str[b * 2 + (b)], "%02x", (unsigned int*)&tmp);
    mac[5 - b] = (uint8_t)tmp;
  }
  //sscanf(mac_str, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", &mac[5], &mac[4], &mac[3], &mac[2], &mac[1], &mac[0]);
  return mac;
}

String MbedSocketClass::macAddress() {
  return String(getNetwork()->get_mac_address());
}

int MbedSocketClass::hostByName(const char* aHostname, IPAddress& aResult) {
  SocketAddress socketAddress = SocketAddress();
  nsapi_error_t returnCode = getNetwork()->gethostbyname(aHostname, &socketAddress);
  nsapi_addr_t address = socketAddress.get_addr();
  aResult[0] = address.bytes[0];
  aResult[1] = address.bytes[1];
  aResult[2] = address.bytes[2];
  aResult[3] = address.bytes[3];
  return returnCode == NSAPI_ERROR_OK ? 1 : 0;
}

IPAddress MbedSocketClass::localIP() {
  SocketAddress ip;
  NetworkInterface* interface = getNetwork();
  interface->get_ip_address(&ip);
  return ipAddressFromSocketAddress(ip);
}

IPAddress MbedSocketClass::subnetMask() {
  SocketAddress ip;
  NetworkInterface* interface = getNetwork();
  interface->get_netmask(&ip);
  return ipAddressFromSocketAddress(ip);
}

IPAddress MbedSocketClass::gatewayIP() {
  SocketAddress ip;
  NetworkInterface* interface = getNetwork();
  interface->get_gateway(&ip);
  return ipAddressFromSocketAddress(ip);
}

IPAddress MbedSocketClass::dnsServerIP() {
  SocketAddress ip;
  NetworkInterface* interface = getNetwork();
  interface->get_dns_server(0, &ip, nullptr);
  return ipAddressFromSocketAddress(ip);
}

IPAddress MbedSocketClass::dnsIP(int n) {
  SocketAddress ip;
  NetworkInterface* interface = getNetwork();
  interface->get_dns_server(n, &ip, nullptr);
  return ipAddressFromSocketAddress(ip);
}

void MbedSocketClass::config(IPAddress local_ip) {
  nsapi_addr_t convertedIP = { NSAPI_IPv4, { local_ip[0], local_ip[1], local_ip[2], local_ip[3] } };
  _ip = SocketAddress(convertedIP);
}

void MbedSocketClass::config(const char* local_ip) {
  _ip = SocketAddress(local_ip);
}

void MbedSocketClass::config(IPAddress local_ip, IPAddress dns_server) {
  config(local_ip);
  setDNS(dns_server);
}

void MbedSocketClass::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway) {
  config(local_ip, dns_server);
  nsapi_addr_t convertedGatewayIP = { NSAPI_IPv4, { gateway[0], gateway[1], gateway[2], gateway[3] } };
  _gateway = SocketAddress(convertedGatewayIP);
}

void MbedSocketClass::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet) {
  config(local_ip, dns_server, gateway);
  nsapi_addr_t convertedSubnetMask = { NSAPI_IPv4, { subnet[0], subnet[1], subnet[2], subnet[3] } };
  _netmask = SocketAddress(convertedSubnetMask);
}

void MbedSocketClass::setDNS(IPAddress dns_server1) {
  nsapi_addr_t convertedDNSServer = { NSAPI_IPv4, { dns_server1[0], dns_server1[1], dns_server1[2], dns_server1[3] } };
  _dnsServer1 = SocketAddress(convertedDNSServer);
}

void MbedSocketClass::setDNS(IPAddress dns_server1, IPAddress dns_server2) {
  setDNS(dns_server1);
  nsapi_addr_t convertedDNSServer2 = { NSAPI_IPv4, { dns_server2[0], dns_server2[1], dns_server2[2], dns_server2[3] } };
  _dnsServer2 = SocketAddress(convertedDNSServer2);
}

IPAddress MbedSocketClass::ipAddressFromSocketAddress(SocketAddress socketAddress) {
  nsapi_addr_t address = socketAddress.get_addr();
  return IPAddress(address.bytes[0], address.bytes[1], address.bytes[2], address.bytes[3]);
}

SocketAddress MbedSocketClass::socketAddressFromIpAddress(IPAddress ip, uint16_t port) {
  nsapi_addr_t convertedIP = { NSAPI_IPv4, { ip[0], ip[1], ip[2], ip[3] } };
  return SocketAddress(convertedIP, port);
}


void MbedSocketClass::setFeedWatchdogFunc(voidFuncPtr func) {
  _feed_watchdog_func = func;
}

void MbedSocketClass::feedWatchdog() {
  if (_feed_watchdog_func)
    _feed_watchdog_func();
}

void MbedSocketClass::body_callback(const char* data, uint32_t data_len) {
  feedWatchdog();
  fwrite(data, sizeof(data[0]), data_len, download_target);
}

int MbedSocketClass::download(const char* url, const char* target_file, bool const is_https) {
  download_target = fopen(target_file, "wb");

  int res = this->download(url, is_https, ::mbed::callback(this, &MbedSocketClass::body_callback));

  fclose(download_target);
  download_target = nullptr;

  return res;
}

// Download helper
int MbedSocketClass::download(const char* url, bool const is_https, mbed::Callback<void(const char*, uint32_t)> cbk) {
  if(cbk == nullptr) {
    return 0; // a call back must be set
  }

  HttpRequest* req_http = nullptr;
  HttpsRequest* req_https = nullptr;
  HttpResponse* rsp = nullptr;
  int res=0;
  std::vector<string*> header_fields;

  if (is_https) {
    req_https = new HttpsRequest(getNetwork(), nullptr, HTTP_GET, url, cbk);
    rsp = req_https->send(NULL, 0);
    if (rsp == NULL) {
      res = req_https->get_error();
      goto exit;
    }
  } else {
    req_http = new HttpRequest(getNetwork(), HTTP_GET, url, cbk);
    rsp = req_http->send(NULL, 0);
    if (rsp == NULL) {
      res = req_http->get_error();
      goto exit;
    }
  }

  while (!rsp->is_message_complete()) {
    delay(10);
  }

  // find the header containing the "Content-Length" value and return that
  header_fields = rsp->get_headers_fields();
  for(size_t i=0; i<header_fields.size(); i++) {

    if(strcmp(header_fields[i]->c_str(), "Content-Length") == 0) {
      res = std::stoi(*rsp->get_headers_values()[i]);
      break;
    }
  }

exit:
  if(req_http)  delete req_http;
  if(req_https) delete req_https;
  // no need to delete rsp, it is already deleted by deleting the request
  // this may be harmful since it can allow dangling pointers existence

  return res;
}
}