#include <iostream>
#include <cstdio>
#include <map>
#include "libusb.h"
using namespace std;

enum Consts
{
    MAX_BUFFER_SIZE = 256
};

map<string, string> classesOfUSB = {make_pair("00h", "NO CODE"), make_pair("01h", "AUDIO"), make_pair("02h", "NETWORK ADAPTER"),
                                    make_pair("03h", "HUMAN INTERFACE DEVICE"), make_pair("05h", "PHYSICAL DEVICE"), make_pair("06h", "IMAGE"),
                                    make_pair("07h", "PRINTER"), make_pair("08h", "STORAGE DEVICE"), make_pair("09h", "CONCENTRATOR/HUB"),
                                    make_pair("0Ah", "CDC-DATA"), make_pair("0Bh", "SMART CARD"), make_pair("0Dh", "CONTENT SECURITY"),
                                    make_pair("0Eh", "VIDEO"), make_pair("0Fh", "PERSONAL HEALTHCARE"), make_pair("10h", "AUDIO/VIDEO DEVICES"),
                                    make_pair("11h", "BILLBOARD DEVICE"), make_pair("12h", "USB TYPE-C BRIDGE DEVICE"), make_pair("3Ch", "I3C DEVICE"),
                                    make_pair("DCh", "DIAGNOSTIC DEVICE"), make_pair("E0h", "WIRELESS CONTROLLER"), make_pair("EFh", "MISCELLANEOUS"),
                                    make_pair("FEh", "APPLICATION SPECIFIC"), make_pair("FFh", "VENDOR SPECIFIC")};

void Swap(char& first, char& second)
{
    char temporary = first;
    first = second;
    second = temporary;
}

void ReverseString(string& string)
{
    for (int i = 0; i < string.size() / 2; ++i)
    {
        Swap(string[i], string[string.size() - i - 1]);
    }
}

void ToHexString(int value, string& result)
{
    string hexDigits = "0123456789ABCDEF";
    while (value > 0)
    {
        result += hexDigits[value % 16];
        value /= 16;
    }

    while (result.size() < 2)
    {
        result.push_back('0');
    }

    ReverseString(result);
    result += 'h';
}

void SetBadSerialNumber(unsigned char* serialNumber)
{
    string badNumber = "bad serial number :(";
    for (int i = 0; i < badNumber.size(); ++i)
    {
        serialNumber[i] = badNumber[i];
    }
}

void PrintUSBClass(string& classCode)
{
    auto classPair = classesOfUSB.find(classCode);
    cout << ">>>" << classPair->second << "<<<" << endl;
}

void PrintDevises(libusb_device *devises)
{
    libusb_device_descriptor description;
    libusb_config_descriptor *config;
    libusb_device_handle *deviceHandle;
    const libusb_interface *interface;
    const libusb_interface_descriptor *interfaceDescription;
    const libusb_endpoint_descriptor *endpointDescription;
    int codeOfInitialisation = libusb_get_device_descriptor(devises, &description);
    if (codeOfInitialisation < 0)
    {
        cout << "initialisation failed with code " << codeOfInitialisation;
        return;
    }

    int codeOfOpen = libusb_open(devises, &deviceHandle);
    if (codeOfOpen != 0)
    {
        cout << "can't open this device" << endl;
        return;
    }

    string classCode;
    ToHexString(static_cast<int>(description.bDeviceClass), classCode);
    libusb_get_config_descriptor(devises, 0, &config);
    unsigned char serialNumber[MAX_BUFFER_SIZE] = {};
    int readingSymbols = libusb_get_string_descriptor_ascii(deviceHandle, description.iSerialNumber, serialNumber, MAX_BUFFER_SIZE);
    if (readingSymbols < 0)
    {
        SetBadSerialNumber(serialNumber);
    }

    cout << ":configuration number: " << static_cast<int>(description.bNumConfigurations) << endl << ":class code: " <<  classCode << endl << ":serial number: " << serialNumber << endl;
    cout << ":vendor ID = " << description.idVendor << ", :product ID = " << description.idProduct << endl << ":number of supported interfaces: " << static_cast<int>(config->bNumInterfaces) << endl;
    PrintUSBClass(classCode);
    for (int i = 0; i < static_cast<int>(config->bNumInterfaces); ++i)
    {
        interface = &config->interface[i];
        cout << "number of alternative settings: " << interface->num_altsetting << endl;
        for (int j = 0; j < interface->num_altsetting; ++j)
        {
            interfaceDescription = &interface->altsetting[j];
            cout << "1) current interface number: " << static_cast<int>(interfaceDescription->bInterfaceNumber) << endl << "2) number of endpoints: " << static_cast<int>(interfaceDescription->bNumEndpoints) << endl;
            for (int k = 0; k < static_cast<int>(interfaceDescription->bNumEndpoints); ++k)
            {
                endpointDescription = &interfaceDescription->endpoint[k];
                cout << "    2.1) descriptor type: " << static_cast<int>(endpointDescription->bDescriptorType) << endl <<"    2.2) endpoint address: " << static_cast<int>(endpointDescription->bEndpointAddress) << endl;
            }
            cout << "=========================================" << endl;
        }
    }

    libusb_close(deviceHandle);
    libusb_free_config_descriptor(config);
}

int main()
{
    libusb_device **devices;
    libusb_context *context = NULL;
    int codeOfInitialisation = libusb_init(&context);;
    if (codeOfInitialisation < 0)
    {
        cout << "initialisation failed with code " << codeOfInitialisation << endl;
        return -1;
    }

    libusb_set_option(context, LIBUSB_OPTION_LOG_LEVEL);
    ssize_t numberOfDevices = libusb_get_device_list(context, &devices);
    if (numberOfDevices < 0)
    {
        cout << "can't get list of devices" << endl;
        return -1;
    }
    
    cout << "number of devises: " << numberOfDevices << endl;
    for(ssize_t i = 0; i < numberOfDevices; i++)
    {
        cout << "NEXT DEVICE: "<< endl;
        PrintDevises(devices[i]);
        cout << endl << endl;
    }

    libusb_free_device_list(devices, 1);
    libusb_exit(context);
    return 0; 
}
