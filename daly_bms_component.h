#include "esphome.h"
#include "esphome/components/mqtt/mqtt_client.h"
#include "esphome/components/esp32_ble_tracker/ble_client.h"

class DalyBMSComponent : public Component, public esp32_ble_tracker::BLEClientNode {
 public:
  DalyBMSComponent(mqtt::MQTTClient *mqtt_client) : mqtt_client_(mqtt_client) {}

  void setup() override {
    // ����� ���������� �� MAC-������
    this->set_address("40:18:01:01:5B:3B", false); // �������� �� MAC ������ DALY BMS
  }

  void loop() override {
    if (this->is_connected()) {
      // ���������� ������ �� ������ 0000fff2-0000-1000-8000-00805f9b34fb
      this->write_value("0000fff2-0000-1000-8000-00805f9b34fb", (uint8_t*)"\xd2\x03\x00\x00\x00\x3e\xd7\xb9", 8);
      
      // ���� ������ � ������������
      auto value = this->read_value("0000fff2-0000-1000-8000-00805f9b34fb");
      if (value.has_value()) {
        // �������� ����� � ���������� ��� � MQTT
        ESP_LOGI("DalyBMS", "����� �� BMS: %s", value->to_string().c_str());
        this->mqtt_client_->publish("daly_bms/charge", value->to_string().c_str());
      }
    }
  }

 private:
  mqtt::MQTTClient *mqtt_client_;
};
