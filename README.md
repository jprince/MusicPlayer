# Updating the Nano
1. In a Finder window, navigate to Documents/Arduino/libraries/NFCReader
2. In another Finder window, navigate to this project's root directory
3. Copy the files from this project
4. Paste the files into the first Finder window
5. From Arduino IDE, compile the sketch and upload the files to the Nano

# Writing to cards

Spotify supports the following urls:
- spotify:album:25L8ck3KGcmCo3901ztPzR
- spotify:playlist:15mly4Os7BdTWCnBEGylJW
- spotify:artist:7oWSqrgMuIEyH9qp5nu2e5
- spotify:track:7lMIJ4EFWRVoAIPDpeGyM3

The IOS NFC Tools app is used for writing data to cards:
1. Open the app
2. Click "Write"
3. Click "Add a record"
4. Select "Custom URL / URI"
5. Enter your desired url, using the formats specific above
6. Click "Write / XX Bytes"
7. Scan your card

# Troubleshooting
404 when calling play() - This is often caused by Spotify's inability to find the selected device. To resolve it, open the Alexa app and initiate a Spotify song on the device. This somehow resolves the communication issue between the Echo and Spotify.

# Credit
h/t to [Michael Hawkins](https://github.com/makeratplay) for excellent tutorials and providing the bulk of the Spotify API code.
