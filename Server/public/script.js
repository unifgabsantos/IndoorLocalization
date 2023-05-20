var data = [{x: [0],y: [0],mode: 'markers',type: 'scatter',text:"Beacon",name:"Beacon",id:-1}];

function receiver(x,y,id){
    return {
        x: [x],
        y: [y],
        mode: 'markers',
        type: 'scatter',
        id:id,
        text:"Receptor "+id,
        name:"Receptor "+id
    }
}

function addReceivers(x,y,id){
    data.push(receiver(x,y,id))
    Plotly.newPlot('myDiv', data);
    return true;
};

function updateReceiver(x,y,id){
    var index = data.findIndex(object=>object.id===id);
    if(index===undefined || index==-1){
        addReceivers(x,y,id);
        return true;
    }
    data[index].x[0] = x;
    data[index].y[0] = y;
    return true;
};

function updateBeacon(x,y){
    let index = data.findIndex(object=>object.name==="Beacon");
    data[index].x[0] = x;
    data[index].y[0] = y;
};

function getReceivers() {
    var requests = new XMLHttpRequest();
    requests.onreadystatechange = function() {
        if (requests.readyState == 4) {
            payload = JSON.parse(requests.responseText);
            for(let i=0;i<payload.length;i++){
                let x = payload[i].x;
                let y = payload[i].y;
                let id = payload[i].id;
                updateReceiver(x,y,id)
            }
        };
    };
    try {
        requests.open("GET", "http://localhost/GetReceptores");
        requests.send();
    } catch (e) {
        console.log(e);
    }
}

function getBeacon() {
    var requests = new XMLHttpRequest();
    requests.onreadystatechange = function() {
        if (requests.readyState == 4) {
            payload = JSON.parse(requests.responseText);
            updateBeacon(payload.x,payload.y)
        };
    };
    try {
        requests.open("GET", "http://localhost/GetRealPosition");
        requests.send();
    } catch (e) {
        console.log(e);
    }
}

setInterval(()=>{
    getReceivers();
    getBeacon();
    Plotly.newPlot('myDiv', data);
},5000)

Plotly.newPlot('myDiv', data);