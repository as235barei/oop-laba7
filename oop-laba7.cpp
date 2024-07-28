#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

namespace {
  enum class Material { Plastic, Metal, Glass };

  std::ostream& operator<<(std::ostream& os, const Material& material) {
    static const std::vector<std::string> materials = { "Plastic", "Metal", "Glass" };
    return os << materials[static_cast<int>(material)];
  }

  std::istream& operator>>(std::istream& is, Material& material) {
    int userChoice;
    is >> userChoice;
    material = static_cast<Material>(userChoice);
    return is;
  }
}

class MeasurementDevice {
protected:
  std::string name;
  std::string unit;
  double minValue;
  double maxValue;
  Material material;
  bool isDeviceActiveForMeasurement;

public:
  MeasurementDevice(const std::string& name, const std::string& unit, double minValue, double maxValue, Material material)
    : name(name), unit(unit), minValue(minValue), maxValue(maxValue), material(material), isDeviceActiveForMeasurement(false) {}

  virtual void startMeasuring() {
    if (!isDeviceActiveForMeasurement) {
      std::cout << "\nStart of measurement" << std::endl;
      isDeviceActiveForMeasurement = true;
    }
  }

  virtual void stopMeasuring() {
    if (isDeviceActiveForMeasurement) {
      std::cout << "End of measurement\n" << std::endl;
      isDeviceActiveForMeasurement = false;
    }
  }

  virtual void print() const {
    std::cout << "============" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Unit: " << unit << std::endl;
    std::cout << "Min Value: " << minValue << std::endl;
    std::cout << "Max Value: " << maxValue << std::endl;
    std::cout << "Material: " << material << std::endl;
    std::cout << "============" << std::endl;
  }

  void setName(const std::string& newName) {
    name = newName;
  }

  void setUnit(const std::string& newUnit) {
    unit = newUnit;
  }

  void setMinValue(double newMinValue) {
    minValue = newMinValue;
  }

  void setMaxValue(double newMaxValue) {
    maxValue = newMaxValue;
  }

  void setMaterial(Material newMaterial) {
    material = newMaterial;
  }

  Material getMaterial() const {
    return material;
  }

  std::string getName() const {
    return name;
  }

  double getMinValue() const {
    return minValue;
  }

  double getMaxValue() const {
    return maxValue;
  }
};

class TemperatureMeasurementDevice : public MeasurementDevice {
public:
  enum class TemperatureScale { Celsius, Fahrenheit, Kelvin };

private:
  double currentTemperature;
  TemperatureScale temperatureScale;

public:
  TemperatureMeasurementDevice(const std::string& name, const std::string& unit, double minValue, double maxValue, Material material, TemperatureScale scale)
    : MeasurementDevice(name, unit, minValue, maxValue, material), currentTemperature(0.0), temperatureScale(scale) {}

  void startMeasuring() override {
    MeasurementDevice::startMeasuring();
    std::cout << "Temperature measurement started" << std::endl;
  }

  void stopMeasuring() override {
    MeasurementDevice::stopMeasuring();
    std::cout << "Temperature measurement stopped" << std::endl;
  }

  void print() const override {
    MeasurementDevice::print();
    printTemperature();
  }

  void setCurrentTemperature(double newTemperature) {
    if (isDeviceActiveForMeasurement) {
      currentTemperature = newTemperature;
    }
    else {
      std::cout << "Device is not ACTIVE!!!" << std::endl;
    }
  }

  void printTemperature() const {
    std::string scale;

    switch (temperatureScale) {
    case TemperatureScale::Celsius:
      scale = "Celsius";
      break;
    case TemperatureScale::Fahrenheit:
      scale = "Fahrenheit";
      break;
    case TemperatureScale::Kelvin:
      scale = "Kelvin";
      break;
    default:
      scale = "Celsius";
      break;
    }

    std::cout << "Current Temperature: " << convertTemperature(currentTemperature, temperatureScale) << " " << scale << std::endl;
  }

  void setTemperatureScale(TemperatureScale newScale) {
    temperatureScale = newScale;
  }

  double getCurrentTemperature() const {
    return currentTemperature;
  }

  double convertTemperature(double temperature, TemperatureScale scale) const {
    switch (scale) {
    case TemperatureScale::Celsius:
      return temperature;
    case TemperatureScale::Fahrenheit:
      return temperature * 9.0 / 5.0 + 32;
    case TemperatureScale::Kelvin:
      return temperature + 273.15;
    default:
      return temperature;
    }
  }
};

class MeasurementDeviceContainer {
private:
  std::vector<std::unique_ptr<MeasurementDevice>> devices;
  size_t currentDeviceIndex;

public:
  MeasurementDeviceContainer() : currentDeviceIndex(0) {}

  void addDeviceToFront(std::unique_ptr<MeasurementDevice> device) {
    devices.insert(devices.begin(), std::move(device));
  }

  void addDeviceToBack(std::unique_ptr<MeasurementDevice> device) {
    devices.push_back(std::move(device));
  }

  void addDeviceAt(std::unique_ptr<MeasurementDevice> device, size_t index) {
    if (index <= devices.size()) {
      devices.insert(devices.begin() + index, std::move(device));
    }
    else {
      std::cout << "Invalid index. Device not added." << std::endl;
    }
  }

  void switchDevice(size_t index) {
    if (index < devices.size()) {
      currentDeviceIndex = index;
      std::cout << "Switched to device " << index + 1 << std::endl;
    }
    else {
      std::cout << "Invalid device index." << std::endl;
    }
  }

  MeasurementDevice* getCurrentDevice() {
    if (devices.empty()) {
      return nullptr;
    }
    return devices[currentDeviceIndex].get();
  }

  size_t getCurrentDeviceIndex() const {
    return currentDeviceIndex;
  }

  size_t getDeviceCount() const {
    return devices.size();
  }

  void printDevices() const {
    std::cout << "Devices in container:" << std::endl;
    for (size_t i = 0; i < devices.size(); ++i) {
      std::cout << i + 1 << ". ";
      devices[i]->print();
    }
  }

  std::vector<MeasurementDevice*> findDevicesByName(const std::string& searchName) {
    std::vector<MeasurementDevice*> foundDevices;
    for (auto& device : devices) {
      if (device->getName() == searchName) {
        foundDevices.push_back(device.get());
      }
    }
    return foundDevices;
  }

  void sortDevicesByMinValue() {
    std::sort(devices.begin(), devices.end(), [](const std::unique_ptr<MeasurementDevice>& a, const std::unique_ptr<MeasurementDevice>& b) {
      return a->getMinValue() < b->getMinValue();
      });
  }

  void sortDevicesByMaxValue() {
    std::sort(devices.begin(), devices.end(), [](const std::unique_ptr<MeasurementDevice>& a, const std::unique_ptr<MeasurementDevice>& b) {
      return a->getMaxValue() < b->getMaxValue();
      });
  }

  void sortDevicesByTemperature() {
    std::sort(devices.begin(), devices.end(), [](const std::unique_ptr<MeasurementDevice>& a, const std::unique_ptr<MeasurementDevice>& b) {
      if (auto tempDeviceA = dynamic_cast<TemperatureMeasurementDevice*>(a.get())) {
        if (auto tempDeviceB = dynamic_cast<TemperatureMeasurementDevice*>(b.get())) {
          return tempDeviceA->getCurrentTemperature() < tempDeviceB->getCurrentTemperature();
        }
      }
      return false;
      });
  }
};

int main() {
  MeasurementDeviceContainer container;

  char addMore = 'y';

  while (addMore == 'y') {
    std::string deviceType;
    std::cout << "Enter device type (MeasurementDevice / TemperatureMeasurementDevice): ";
    std::cin >> deviceType;

    std::string name, unit;
    double minValue, maxValue;
    Material material;

    std::cout << "Enter device name: ";
    std::cin >> name;
    std::cout << "Enter unit: ";
    std::cin >> unit;
    std::cout << "Enter min value: ";
    std::cin >> minValue;
    std::cout << "Enter max value: ";
    std::cin >> maxValue;
    int materialChoice;
    std::cout << "Enter material (1 for Plastic, 2 for Metal, 3 for Glass): ";
    std::cin >> materialChoice;
    material = static_cast<Material>(materialChoice - 1);

    std::unique_ptr<MeasurementDevice> device;

    if (deviceType == "TemperatureMeasurementDevice") {
      int scaleChoice;
      std::cout << "Choose temperature scale (1 - Celsius, 2 - Fahrenheit, 3 - Kelvin): ";
      std::cin >> scaleChoice;
      TemperatureMeasurementDevice::TemperatureScale scale = static_cast<TemperatureMeasurementDevice::TemperatureScale>(scaleChoice - 1);
      device = std::make_unique<TemperatureMeasurementDevice>(name, unit, minValue, maxValue, material, scale);
    }
    else if (deviceType == "MeasurementDevice") {
      device = std::make_unique<MeasurementDevice>(name, unit, minValue, maxValue, material);
    }
    else {
      std::cout << "Invalid device type. Please choose either MeasurementDevice or TemperatureMeasurementDevice.\n";
      continue;
    }

    container.printDevices();
    std::cout << "Choose position to insert the device (1 - at the beginning, 2 - at the end, 3 - at a specific position): ";
    int insertOption;
    std::cin >> insertOption;

    switch (insertOption) {
    case 1:
      container.addDeviceToFront(std::move(device));
      break;
    case 2:
      container.addDeviceToBack(std::move(device));
      break;
    case 3:
      size_t index;
      std::cout << "Enter position to insert (1 - " << container.getDeviceCount() + 1 << "): ";
      std::cin >> index;
      container.addDeviceAt(std::move(device), index - 1);
      break;
    default:
      std::cout << "Invalid option. Device not inserted." << std::endl;
      break;
    }

    std::cout << "Do you want to add another device? (y/n): ";
    std::cin >> addMore;
  }

  int option;

  do {
    std::cout << "\nChoose an option:\n"
      "1. Switch to next device\n"
      "2. Change device attributes\n"
      "3. Print device info\n"
      "4. Start measuring\n"
      "5. Stop measuring\n"
      "6. Print temperature\n"
      "7. Print all devices\n"
      "8. Add a new device\n"
      "9. Search or sort devices\n"
      "0. Exit\n"
      "Enter option: ";

    std::cin >> option;

    switch (option) {
    case 1:
      container.switchDevice((container.getCurrentDeviceIndex() + 1) % container.getDeviceCount());
      break;
    case 2: {
      int attributeOption;
      std::cout << "Choose attribute to change:\n"
        "1. Name\n"
        "2. Unit\n"
        "3. Min Value\n"
        "4. Max Value\n"
        "5. Material\n"
        "6. Temperature\n"
        "7. Temperature Scale\n"
        "Enter option: ";
      std::cin >> attributeOption;

      switch (attributeOption) {
      case 1: {
        std::string newName;
        std::cout << "Enter new name: ";
        std::cin >> newName;
        container.getCurrentDevice()->setName(newName);
        break;
      }
      case 2: {
        std::string newUnit;
        std::cout << "Enter new unit: ";
        std::cin >> newUnit;
        container.getCurrentDevice()->setUnit(newUnit);
        break;
      }
      case 3: {
        double newMinValue;
        std::cout << "Enter new min value: ";
        std::cin >> newMinValue;
        container.getCurrentDevice()->setMinValue(newMinValue);
        break;
      }
      case 4: {
        double newMaxValue;
        std::cout << "Enter new max value: ";
        std::cin >> newMaxValue;
        container.getCurrentDevice()->setMaxValue(newMaxValue);
        break;
      }
      case 5: {
        int newMaterialChoice;
        std::cout << "Enter new material (1 for Plastic, 2 for Metal, 3 for Glass): ";
        std::cin >> newMaterialChoice;
        Material newMaterial = static_cast<Material>(newMaterialChoice - 1);
        container.getCurrentDevice()->setMaterial(newMaterial);
        break;
      }
      case 6: {
        if (auto temperatureDevice = dynamic_cast<TemperatureMeasurementDevice*>(container.getCurrentDevice())) {
          double newTemperature;
          std::cout << "Enter new temperature: ";
          std::cin >> newTemperature;
          temperatureDevice->setCurrentTemperature(newTemperature);
        }
        else {
          std::cout << "Current device is not a TemperatureMeasurementDevice." << std::endl;
        }
        break;
      }
      case 7: {
        if (auto temperatureDevice = dynamic_cast<TemperatureMeasurementDevice*>(container.getCurrentDevice())) {
          int scaleChoice;
          std::cout << "Choose temperature scale (1 - Celsius, 2 - Fahrenheit, 3 - Kelvin): ";
          std::cin >> scaleChoice;
          temperatureDevice->setTemperatureScale(static_cast<TemperatureMeasurementDevice::TemperatureScale>(scaleChoice - 1));
        }
        else {
          std::cout << "Current device is not a TemperatureMeasurementDevice." << std::endl;
        }
        break;
      }
      case 0:
        break;
      default:
        std::cout << "Invalid option.\n";
      }
      break;
    }
    case 3:
      if (!container.getCurrentDevice()) {
        std::cout << "No device selected." << std::endl;
        break;
      }
      container.getCurrentDevice()->print();
      break;
    case 4:
      container.getCurrentDevice()->startMeasuring();
      break;
    case 5:
      container.getCurrentDevice()->stopMeasuring();
      break;
    case 6:
      if (auto temperatureDevice = dynamic_cast<TemperatureMeasurementDevice*>(container.getCurrentDevice())) {
        temperatureDevice->printTemperature();
      }
      else {
        std::cout << "Current device is not a TemperatureMeasurementDevice." << std::endl;
      }
      break;
    case 7:
      container.printDevices();
      break;
    case 8:
      addMore = 'y';

      while (addMore == 'y') {
        std::string deviceType;
        std::cout << "Enter device type (MeasurementDevice / TemperatureMeasurementDevice): ";
        std::cin >> deviceType;

        std::string name, unit;
        double minValue, maxValue;
        Material material;

        std::cout << "Enter device name: ";
        std::cin >> name;
        std::cout << "Enter unit: ";
        std::cin >> unit;
        std::cout << "Enter min value: ";
        std::cin >> minValue;
        std::cout << "Enter max value: ";
        std::cin >> maxValue;
        int materialChoice;
        std::cout << "Enter material (1 for Plastic, 2 for Metal, 3 for Glass): ";
        std::cin >> materialChoice;
        material = static_cast<Material>(materialChoice - 1);

        std::unique_ptr<MeasurementDevice> device;

        if (deviceType == "TemperatureMeasurementDevice") {
          int scaleChoice;
          std::cout << "Choose temperature scale (1 - Celsius, 2 - Fahrenheit, 3 - Kelvin): ";
          std::cin >> scaleChoice;
          TemperatureMeasurementDevice::TemperatureScale scale = static_cast<TemperatureMeasurementDevice::TemperatureScale>(scaleChoice - 1);
          device = std::make_unique<TemperatureMeasurementDevice>(name, unit, minValue, maxValue, material, scale);
        }
        else if (deviceType == "MeasurementDevice") {
          device = std::make_unique<MeasurementDevice>(name, unit, minValue, maxValue, material);
        }
        else {
          std::cout << "Invalid device type. Please choose either MeasurementDevice or TemperatureMeasurementDevice.\n";
          continue;
        }

        container.printDevices();
        std::cout << "Choose position to insert the device (1 - at the beginning, 2 - at the end, 3 - at a specific position): ";
        int insertOption;
        std::cin >> insertOption;

        switch (insertOption) {
        case 1:
          container.addDeviceToFront(std::move(device));
          break;
        case 2:
          container.addDeviceToBack(std::move(device));
          break;
        case 3:
          size_t index;
          std::cout << "Enter position to insert (1 - " << container.getDeviceCount() + 1 << "): ";
          std::cin >> index;
          container.addDeviceAt(std::move(device), index - 1);
          break;
        default:
          std::cout << "Invalid option. Device not inserted." << std::endl;
          break;
        }

        std::cout << "Do you want to add another device? (y/n): ";
        std::cin >> addMore;
      }
      break;
    case 9: {
      int searchOption;
      std::cout << "Choose search or sort option:\n"
        "1. Find devices by name\n"
        "2. Sort devices by min value\n"
        "3. Sort devices by max value\n"
        "4. Sort devices by temperature\n"
        "Enter option: ";
      std::cin >> searchOption;

      switch (searchOption) {
      case 1: {
        std::string searchName;
        std::cout << "Enter device name to search: ";
        std::cin >> searchName;
        auto foundDevices = container.findDevicesByName(searchName);
        if (foundDevices.empty()) {
          std::cout << "No devices found with the given name." << std::endl;
        }
        else {
          std::cout << "Devices found with the name '" << searchName << "':" << std::endl;
          for (auto device : foundDevices) {
            device->print();
          }
        }
        break;
      }
      case 2:
        container.sortDevicesByMinValue();
        std::cout << "Devices sorted by min value." << std::endl;
        break;
      case 3:
        container.sortDevicesByMaxValue();
        std::cout << "Devices sorted by max value." << std::endl;
        break;
      case 4:
        container.sortDevicesByTemperature();
        std::cout << "Devices sorted by temperature." << std::endl;
        break;
      default:
        std::cout << "Invalid option." << std::endl;
      }
      break;
    }
    case 0:
      std::cout << "Exiting program." << std::endl;
      break;
    default:
      std::cout << "Invalid option.\n";
    }

  } while (option != 0);

  return 0;
}
