#pragma once

#include "ICommunication.hpp"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <cstring>
#include <deque>
#include <mutex>

class BLESerialCommunication : public ICommunication, public BLEServerCallbacks, public BLECharacteristicCallbacks {
 private:
  bool m_isOpen;
  BLEServer* m_Server;
  BLEService* m_Service;
  BLECharacteristic* m_txCharacteristic;
  BLECharacteristic* m_rxCharacteristic;
  std::deque<std::string> m_ReadBufer;
  std::mutex m_BufferLock;

 public:
  BLESerialCommunication() :
      m_isOpen(false),
      m_Server(nullptr),
      m_Service(nullptr),
      m_txCharacteristic(nullptr),
      m_rxCharacteristic(nullptr) {}

  ~BLESerialCommunication() {
    if (m_rxCharacteristic != nullptr) {
      delete m_rxCharacteristic;
      m_rxCharacteristic = nullptr;
    }

    if (m_txCharacteristic != nullptr) {
      delete m_txCharacteristic;
      m_txCharacteristic = nullptr;
    }

    if (m_Service != nullptr) {
      delete m_Service;
      m_Service = nullptr;
    }

    if (m_Server != nullptr) {
      delete m_Server;
      m_Server = nullptr;
    }
  }

  bool isOpen() {
    return m_isOpen;
  }

  void start() {
    // Create the BLE Device
    BLEDevice::init(BT_DEVICE_NAME);

    // Create the BLE Server
    m_Server = BLEDevice::createServer();

    // Set the callbacks of the Server.
    m_Server->setCallbacks(this);

    // Create the BLE Service
    m_Service = m_Server->createService(BLE_UUID);

    // Create a BLE TX Characteristic
    m_txCharacteristic = m_Service->createCharacteristic(
        BLE_TX_UUID, BLECharacteristic::PROPERTY_NOTIFY);
    m_txCharacteristic->addDescriptor(new BLE2902());

    // Create a BLE RX Characteristic
    m_rxCharacteristic = m_Service->createCharacteristic(
      BLE_RX_UUID, BLECharacteristic::PROPERTY_WRITE);
    m_rxCharacteristic->setCallbacks(this);

    // Start the service
    m_Service->start();

    // Start advertising
    m_Server->getAdvertising()->start();
  }

  void output(char* data) {
    m_txCharacteristic->setValue(std::string(data));
    m_txCharacteristic->notify();
  }

  bool hasData() override {
    const std::lock_guard<std::mutex> lock(m_BufferLock);
    return !m_ReadBufer.empty();
  }

  bool readData(char* input, size_t buffer_size) {
    // Lock the buffer to protect it from concurrent modification.
    const std::lock_guard<std::mutex> lock(m_BufferLock);
    if (m_ReadBufer.empty()) {
      return false;
    }

    // Limit the string to the buffer size by taking the minimum.
    size_t size = std::min(m_ReadBufer.front().size(), buffer_size);

    // Copy to the buffer.
    std::memcpy(input, m_ReadBufer.front().data(), size);

    // Null terminate the string.
    input[size] = '\0';

    // Remove the element from the buffer.
    m_ReadBufer.pop_front();
    return size > 0;
  }

  // Callbacks for BLE actions.
  void onConnect(BLEServer* /* server */) override { m_isOpen = true; };
  void onDisconnect(BLEServer* /* server */) override { m_isOpen = false; }

  void onWrite(BLECharacteristic* ble_characteristic) {
    std::string rx_value = ble_characteristic->getValue();

    // Push the string into the buffer.
    const std::lock_guard<std::mutex> lock(m_BufferLock);
    m_ReadBufer.emplace_back(std::move(rx_value));
  }
};
