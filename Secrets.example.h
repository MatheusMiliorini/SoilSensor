#ifndef SECRETS_H
#define SECRETS_H

const char* SSID_1 = "";
const char* SSID_2 = "";
const char* PASSWORD = "";

const uint8_t TIMEZONE = 0;

const char* WEBHOOK_URL = "";

const char* PLANT_NAME = "";
const char* PLANT_LOCATION = "";

const char* AWS_IOT_PUBLISH_TOPIC = "";

const char* MQTT_HOST = "";

const char* THING_NAME = "";

static const char* CA_CERT PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";


// Copy contents from XXXXXXXX-certificate.pem.crt here ▼
static const char* CLIENT_CERT PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";


// Copy contents from  XXXXXXXX-private.pem.key here ▼
static const char* PRIVATE_KEY PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)KEY";

#endif