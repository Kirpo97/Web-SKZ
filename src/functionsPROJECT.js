const char FUNCTIONS[] PROGMEM = R"=====(

setInterval(
    function() {
        getJson();
    }, 500);

function getJson() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var obj = JSON.parse(xhttp.responseText);
            document.getElementById('U').innerHTML = obj.Uout;
            document.getElementById('I').innerHTML = obj.Iout;
            document.getElementById('Upp').innerHTML = obj.Upp;
            document.getElementById('Usp').innerHTML = obj.Usp;
            document.getElementById('label2').innerHTML = obj.label2;
            document.getElementById('label3').innerHTML = obj.label3;
            document.getElementById('label4').innerHTML = obj.label4;
            document.getElementById('label5').innerHTML = obj.label5;
            document.getElementById('label6').innerHTML = obj.label6;
        }
    };
    xhttp.open('GET', '/ConfigJSON', true);
    xhttp.send();
}


function button1() {
    var xhttp = new XMLHttpRequest();

    document.getElementById('div1').innerHTML =
        "<p> <input class='form-control w-100' min='0.0' max='99.9' size='40' type='number' step='0.1' title='Значения вводятся в интервале [ 0,0 ... 99,9 ] через запятую!' placeholder='[ 0,0 ... 99,9 ]' name='num' id='num' style='height:50px; font-size: 150% '></p>";
    document.getElementById('div2').innerHTML =
        "<p> <input class='btn btn-primary rounded w-100' size='10' type='submit' value='Установить' style='height:50px; width: 650px; font-size: 150% '> </p>";

    document.querySelector("#btn1").style.backgroundColor = "#FFFFFF";
    document.querySelector("#btn1").style.color = "#000000";
    document.querySelector("#btn1").style.color = "#000000";
    document.querySelector("#btn2").style.backgroundColor = "#000000";
    document.querySelector("#btn2").style.color = "#FFFFFF";
    document.querySelector("#btn3").style.backgroundColor = "#000000";
    document.querySelector("#btn3").style.color = "#FFFFFF";
    document.querySelector("#btn4").style.backgroundColor = "#000000";
    document.querySelector("#btn4").style.color = "#FFFFFF";

    xhttp.open('GET', '/Button_Uout', true);
    xhttp.send();
}

function button2() {
    var xhttp = new XMLHttpRequest();

    document.getElementById('div1').innerHTML =
        "<p> <input class='form-control w-100' min='0.0' max='999.9' size='40' type='number' step='0.1' title='Значения вводятся в интервале [ 0,0 ... 999,9 ] через запятую!' placeholder='[ 0,0 ... 999,9 ]' name='num' id='num' style='height:50px; font-size: 150% '> </p>"
    document.getElementById('div2').innerHTML =
        "<p> <input class='btn btn-primary rounded w-100' size='10' type='submit' value='Установить' style='height:50px; width: 650px; font-size: 150% '> </p> </p>";

    document.querySelector("#btn1").style.backgroundColor = "#000000";
    document.querySelector("#btn1").style.color = "#FFFFFF";
    document.querySelector("#btn2").style.backgroundColor = "#FFFFFF";
    document.querySelector("#btn2").style.color = "#000000";
    document.querySelector("#btn3").style.backgroundColor = "#000000";
    document.querySelector("#btn3").style.color = "#FFFFFF";
    document.querySelector("#btn4").style.backgroundColor = "#000000";
    document.querySelector("#btn4").style.color = "#FFFFFF";

    xhttp.open('GET', '/Button_Iout', true);
    xhttp.send();
}

function button3() {
    var xhttp = new XMLHttpRequest();

    document.getElementById('div1').innerHTML =
        "<p> <input class='form-control w-100' min='-5.00' max='-0.50' size='40' type='number' step='0.01' title='Значения вводятся в интервале [ -5,00 ... -0,50 ] через запятую!' placeholder='[ -5,00 ... -0,50 ]' name='num' id='num' style='height:50px; font-size: 150% '></p>"
    document.getElementById('div2').innerHTML =
        "<p> <input class='btn btn-primary rounded w-100' size='10' type='submit' value='Установить' style='height:50px; width: 650px; font-size: 150% '> </p>";
    document.querySelector("#btn1").style.backgroundColor = "#000000";
    document.querySelector("#btn1").style.color = "#FFFFFF";
    document.querySelector("#btn2").style.backgroundColor = "#000000";
    document.querySelector("#btn2").style.color = "#FFFFFF";
    document.querySelector("#btn3").style.backgroundColor = "#FFFFFF";
    document.querySelector("#btn3").style.color = "#000000";
    document.querySelector("#btn4").style.backgroundColor = "#000000";
    document.querySelector("#btn4").style.color = "#FFFFFF";

    xhttp.open('GET', '/Button_Upp', true);
    xhttp.send();
}

function button4() {
    var xhttp = new XMLHttpRequest();

    document.getElementById('div1').innerHTML = 
        "<p> <input class='form-control w-100' min='-5.00' max='-0.50' size='40' type='number' step='0.01' title='Значения вводятся в интервале [ -5,00 ... -0,50 ] через запятую!' placeholder='[ -5,00 ... -0,50 ]' name='num' id='num' style='height:50px; font-size: 150% '></p>"
    document.getElementById('div2').innerHTML =
        "<p> <input class='btn btn-primary rounded w-100' size='10' type='submit' value='Установить' style='height:50px; width: 650px; font-size: 150% '> </p>";
    document.querySelector("#btn1").style.backgroundColor = "#000000";
    document.querySelector("#btn1").style.color = "#FFFFFF";
    document.querySelector("#btn2").style.backgroundColor = "#000000";
    document.querySelector("#btn2").style.color = "#FFFFFF";
    document.querySelector("#btn3").style.backgroundColor = "#000000";
    document.querySelector("#btn3").style.color = "#FFFFFF";
    document.querySelector("#btn4").style.backgroundColor = "#FFFFFF";
    document.querySelector("#btn4").style.color = "#000000";

    xhttp.open('GET', '/Button_Usp', true);
    xhttp.send();
}
)=====";