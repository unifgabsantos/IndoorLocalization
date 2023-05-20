import requests
def getRealPosition():
    response = requests.get("http://localhost/GetRealPosition/").text
    return response
def sendRSSI(mac:str,rssi:str):
    payload = {
        "scans": [{"detectedRSSI": rssi,"detectedMAC":"7C:9E:BD:F0:7B:26"}],
        "scannerMAC": mac
    }
    response = requests.post("http://localhost/beaconScan/",json=payload).text
    return response

print(sendRSSI("C8:F0:9E:4F:3C:2E","-50"))
print(sendRSSI("C8:C9:A3:E0:48:4A","-60"))
print(sendRSSI("C8:F0:9E:4F:AA:6A","-55"))
print(sendRSSI("C8:F0:9E:4A:76:12","-45"))
print(getRealPosition())