setInterval(function () {
    getBMETemperatureData();
    getBMEHumidityData();
    getBMEPressureData();
    getBMPTemperatureData();
    getDHTHumidityData();
    getBMPPressureData();
    getWindDirectionData();
    getLightData();
    getUVData();
    getPrecipitationData();
    getWindSpeedData();
}, 60000);

setInterval(function() {
// Call a function repetatively with 1 Second interval
    Time_Date();
}, 1000); 

// BME Temperature
function getBMETemperatureData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("BMETemperatureValue").innerHTML =
                this.responseText;
        }
    };
    xhttp.open("GET", "readBMETemperature", true);
    xhttp.send();
}
// BME Humidity
function getBMEHumidityData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("BMEHumidityValue").innerHTML =
                this.responseText;
        }
    };
    xhttp.open("GET", "readBMEHumidity", true);
    xhttp.send();
}

// BME Pressure
function getBMEPressureData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("BMEPressureValue").innerHTML =
                this.responseText;
        }
    };
    xhttp.open("GET", "readBMEPressure", true);
    xhttp.send();
}

// BMP Temperature
function getBMPTemperatureData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("BMPTemperatureValue").innerHTML =
                this.responseText;
        }
    };
    xhttp.open("GET", "readBMPTemperature", true);
    xhttp.send();
}

// DHT Humidity
function getDHTHumidityData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("DHTHumidityValue").innerHTML =
                this.responseText;
        }
    };
    xhttp.open("GET", "readDHTHumidity", true);
    xhttp.send();
}

// BMP Pressure
function getBMPPressureData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("BMPPressureValue").innerHTML = 
                this.responseText;
        }
    };
    xhttp.open("GET", "readBMPPressure", true);
    xhttp.send();
}

// Wind Direction
function getWindDirectionData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("WindDirectionValue").innerHTML = 
                this.responseText;
        }
    };
    xhttp.open("GET", "readWindDirection", true);
    xhttp.send();
}

// Light
function getLightData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("LightValue").innerHTML =
                this.responseText;
        }
    };
    xhttp.open("GET", "readLight", true);
    xhttp.send();
}

// UV
function getUVData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("UVValue").innerHTML = 
                this.responseText;
        }
    };
    xhttp.open("GET", "readUV", true);
    xhttp.send();
}

// Precipitation
function getPrecipitationData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("PrecipitationValue").innerHTML =
                this.responseText;
        }
    };
    xhttp.open("GET", "readPrecipitation", true);
    xhttp.send();
}

// Wind Speed
function getWindSpeedData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("WindSpeedValue").innerHTML =
                this.responseText;
        }
    };
    xhttp.open("GET", "readWindSpeed", true);
    xhttp.send();
}

// Time and Date
function Time_Date() {
    var t = new Date();
    document.getElementById("time").innerHTML = t.toLocaleTimeString();
    var d = new Date();
    const dayNames = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday","Saturday"];
    const monthNames = ["January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"];
    document.getElementById("date").innerHTML = dayNames[d.getDay()] + ", " + d.getDate() + "-" + monthNames[d.getMonth()] + "-" + d.getFullYear();
}

// Download CSV file
function downloadCSV() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var blob = new Blob([this.responseText], { type: 'text/csv' });
            var url = window.URL.createObjectURL(blob);
            var a = document.createElement('a');
            a.href = url;
            a.download = 'data.csv';
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);
        }
    };
    xhttp.open("GET", "downloadCSV", true);
    xhttp.send();
}

// Refresh Data
function refreshData() {
    getBMETemperatureData();
    getBMEHumidityData();
    getBMEPressureData();
    getBMPTemperatureData();
    getBMPPressureData();
    getDHTHumidityData();
    getWindDirectionData();
    getLightData();
    getUVData();
    getPrecipitationData();
    getWindSpeedData();
}