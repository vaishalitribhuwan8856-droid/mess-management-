# RFID Mess Management — Mobile APK build (मराठी)

## काय आहे?
हा Android WebView app source project आहे. अॅप ESP8266 च्या `http://192.168.4.1` API शी जोडतो. APK स्वतः तयार झालेला नाही; खाली GitHub Actions वापरून फोनवरून cloud build करता येईल.

## मोबाईलवरून APK तयार करण्याची सोपी पद्धत (GitHub Actions)
1. ZIP extract करा.
2. GitHub वर account/login करा: https://github.com
3. New repository तयार करा (उदा. `rfid-mess-app`).
4. या ZIP मधील सर्व files/folders repository मध्ये upload करा. `.github/workflows/build-apk.yml` हा folder सुद्धा upload झाला पाहिजे.
5. Repository मध्ये **Actions** tab उघडा. Workflow `Build RFID Mess APK` निवडा आणि **Run workflow** दाबा. (किंवा main branch वर files upload केल्यावर push workflow सुरू होईल.)
6. Build पूर्ण झाल्यावर workflow run उघडा. खाली **Artifacts** मधील `RFID-Mess-Management-APK` डाउनलोड करा.
7. ZIP extract करून `app-debug.apk` उघडा आणि Install करा. Android ने विचारल्यास त्या browser/file manager साठी “Install unknown apps” परवानगी द्या.

## Hardware
- RC522 -> UNO: SDA/SS=D10, SCK=D13, MOSI=D11, MISO=D12, RST=D9, GND=GND, VCC=3.3V.
- RC522 ला UNO च्या 5V signals पासून योग्य level shifter वापरून सुरक्षित ठेवा.
- UNO D3 TX -> ESP8266 D5/RX voltage divider मधून; GND common. ESP D6/TX -> UNO D2/RX optional.
- ESP8266 ला USB/stable supply द्या; ESP8266 pins 5V tolerant नाहीत.
- ESP8266 AP: `RFID_MESS`, password `mess12345`, address `192.168.4.1`.

## महत्त्वाच्या मर्यादा
हा starter prototype आहे, production-ready नाही. सध्याच्या ESP sketch मध्ये meal/date handling basic आहे आणि app मधील meal selector अजून ESP API ला पाठवत नाही. वास्तविक daily/monthly meal reports व Breakfast/Lunch/Dinner नोंदणीसाठी पुढील सुधारणा आणि hardware testing आवश्यक आहे. Admin PIN 1234 हा साधा local lock आहे; deployment आधी बदला. ESP8266 flash मध्ये data साठतो; flash format केल्यास data नष्ट होऊ शकतो.
