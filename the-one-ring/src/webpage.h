#ifndef WEBPAGE_H
#define WEBPAGE_H

const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>ESP8266 LED Control</title>
    <style>
        body {
            font-family: Arial, sans-serif;
        }
        .container {
            max-width: 500px;
            margin: auto;
            padding: 10px;
            text-align: center;
        }
        .input-group {
            margin-bottom: 10px;
        }
        .input-group label {
            display: block;
            margin-bottom: 5px;
        }
        .input-group input, .input-group select {
            width: 100%;
            padding: 8px;
            box-sizing: border-box;
        }
        .input-group button {
            padding: 10px 20px;
            cursor: pointer;
        }
    </style>
    <script>
        function sendRequest(url, callback) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", url, true);
            xhr.onreadystatechange = function () {
                if (xhr.readyState == 4 && xhr.status == 200) {
                    callback(xhr.responseText);
                }
            };
            xhr.send();
        }

        function setBrightness() {
            var brightness = document.getElementById('brightness').value;
            var strip = document.getElementById('strip').value;
            sendRequest('/setbrightness?val=' + brightness + '&strip=' + strip, function(response) {
                alert(response);
            });
        }

        function setColor() {
            var r = document.getElementById('colorR').value;
            var g = document.getElementById('colorG').value;
            var b = document.getElementById('colorB').value;
            var strip = document.getElementById('strip').value;
            sendRequest('/setcolor?R=' + r + '&G=' + g + '&B=' + b + '&strip=' + strip, function(response) {
                alert(response);
            });
        }

        function setAnimation() {
            var animation = document.getElementById('animation').value;
            var strip = document.getElementById('strip').value;
            sendRequest('/setanimation?id=' + animation + '&strip=' + strip, function(response) {
                alert(response);
            });
        }

        function setAniMod() {
            var animod = document.getElementById('animod').value;
            var strip = document.getElementById('strip').value;
            sendRequest('/setanimod?val1=' + animod + '&strip=' + strip, function(response) {
                alert(response);
            });
        }

        function setLED() {
            var strip = document.getElementById('strip').value;
            var ledIndex = document.getElementById('ledIndex').value;
            var r = document.getElementById('ledR').value;
            var g = document.getElementById('ledG').value;
            var b = document.getElementById('ledB').value;
            sendRequest('/setled?strip=' + strip + '&led=' + ledIndex + '&R=' + r + '&G=' + g + '&B=' + b, function(response) {
                alert(response);
            });
        }
    </script>
</head>
<body>
    <div class="container">
        <h1>ESP8266 LED Control</h1>
        <div class="input-group">
            <label for="brightness">Set Brightness (0-255):</label>
            <input type="number" id="brightness" min="0" max="255" />
            <label for="strip">Strip Index (0 or 1):</label>
            <input type="number" id="strip" min="0" max="1" />
            <button onclick="setBrightness()">Set Brightness</button>
        </div>
        <div class="input-group">
            <label for="colorR">Set Color - Red (0-255):</label>
            <input type="number" id="colorR" min="0" max="255" />
            <label for="colorG">Set Color - Green (0-255):</label>
            <input type="number" id="colorG" min="0" max="255" />
            <label for="colorB">Set Color - Blue (0-255):</label>
            <input type="number" id="colorB" min="0" max="255" />
            <button onclick="setColor()">Set Color</button>
        </div>
        <div class="input-group">
            <label for="animation">Set Animation:</label>
            <select id="animation">
                <option value="0">Color Roll</option>
                <option value="1">Full On</option>
                <option value="2">Color Follow</option>
                <option value="3">Glow</option>
                <option value="4">Custom LEDs</option>
                <option value="5">LAVA_FLOW</option>
            </select>
            <button onclick="setAnimation()">Set Animation</button>
        </div>
        <div class="input-group">
            <label for="animod">Set Animation Modifier (0-255):</label>
            <input type="number" id="animod" min="0" max="255" />
            <button onclick="setAniMod()">Set Animation Modifier</button>
        </div>
        <div class="input-group">
            <label for="ledIndex">Set LED:</label>
            <input type="number" id="ledIndex" min="0" max="255" placeholder="LED Index" />
            <input type="number" id="ledR" min="0" max="255" placeholder="Red" />
            <input type="number" id="ledG" min="0" max="255" placeholder="Green" />
            <input type="number" id="ledB" min="0" max="255" placeholder="Blue" />
            <button onclick="setLED()">Set LED</button>
        </div>
    </div>
</body>
</html>
)=====";

#endif // WEBPAGE_H
