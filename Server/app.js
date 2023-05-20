const express = require("express");
const bodyParser = require('body-parser');
const app = express();
const fs = require("fs")
var trilateration = require("trilateration");
app.use('/', express.static('public'));
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

trilateration.addBeacon(0, trilateration.vector(2, 4));
trilateration.addBeacon(1, trilateration.vector(5.5, 13));
trilateration.addBeacon(2, trilateration.vector(11.5, 2));
trilateration.addBeacon(3, trilateration.vector(99, 99));

function rssiToDistance(rssi,id) {
  var txPower = -76;
  if(id==0){
    txPower = -79
  }else if(id==1){
    txPower = -85
  }
  else if(id==2){
    txPower = -76
  }
  else if(id==3){
    txPower = -87
  }
    const attenuationConstant = 2.0; // para o ar em ambiente interno
    const distance = 10 ** ((txPower - rssi) / (10 * attenuationConstant));
    return distance;
};

function writeData(id,data){
  data = data+"\n";
  let fileName = "./Dados/"+id+".txt"
  fs.appendFile (fileName, data, (err) => {if (err) throw err;});
}

app.post("/beaconScan",(req,resp)=>{
    const payload = req.body;
    var rssi = 0;
    var distance = 0;
    var id;
    if(payload.scannerMAC=="C8:F0:9E:4F:3C:2E"){
      id = 0;
    }
    else if(payload.scannerMAC=="C8:C9:A3:E0:48:4A"){
      id = 1;
    }
    else if(payload.scannerMAC=="C8:F0:9E:4F:AA:6A"){
      id = 2;
    }
    else if(payload.scannerMAC=="C8:F0:9E:4A:76:12"){
      id = 3;
    }
    else{
      resp.status(404).send("MAC address not found");
      return;
    }
    for(let i=0;i<payload.scans.length;i++){
      if(payload.scans[i].detectedMAC == "7C:9E:BD:F0:7B:26"){
          rssi = parseFloat(payload.scans[i].detectedRSSI);
          distance = rssiToDistance(rssi,id)
          console.log(distance + " - " + rssi)
      }
  }

  let x,y,obj;
  obj = trilateration.beacons().find((a)=>a.id-1==id);
  if(obj!=undefined){
    x = obj.x;
    y = obj.y
    data = {
      coordinates:{x,y},
      rssi:rssi,
      distance:distance,
      timestamp:(new Date().valueOf()).toString()
    }
    writeData(id,JSON.stringify(data))
  }
  try{
    DataNotFiltered.push()
  }
  catch{
    null;
  }
  trilateration.setDistance(id,distance);
  resp.status(200).send("OK");
});


app.get("/GetRealPosition/",(req,resp)=>{
    const coordinates = trilateration.calculatePosition();
    resp.status(200).send(coordinates);
});

app.get("/GetReceptores",(req,resp)=>{
    let response = trilateration.beacons();
    console.log(response);
    resp.status(200).send(response);
});
app.listen(80);
