
var newprefix = "cccc";

// load(lib.js)
var prefix = "bbbb";
var brID = 1;

//Wait until all mote started
function WaitingStarting() {
    for(var i=0; i<3; i++) {
        YIELD_THEN_WAIT_UNTIL(msg.indexOf("Contiki>") != -1);
    }
}

// Display all mote Table
function displayAllTable(){
    var allm = sim.getMotes();
    for(var id in  allm) {
	    write(allm[id], "netd nc");
	    YIELD_THEN_WAIT_UNTIL(msg.indexOf("Contiki>") != -1);
	    write(allm[id], "netd rt");
	    YIELD_THEN_WAIT_UNTIL(msg.indexOf("Contiki>") != -1);
        write(allm[id], "netd cp");
        YIELD_THEN_WAIT_UNTIL(msg.indexOf("Contiki>") != -1);
        if(allm[id].getID() == brID) {
            write(allm[id], "netd dad");
            YIELD_THEN_WAIT_UNTIL(msg.indexOf("Contiki>") != -1);
        }
	}
}

// Add to routing table
function genip(num, pref){
    var ip = "";
    ip += pref;
    for(var i=0; i<3; i++){
        ip +=":0000";
    }
    var n;
    ip += ":0212";
    n = 0x7400 + num;
    ip += ":"+n.toString(16);
    n = 0x0 + num;
    ip += ":000"+n.toString(16);
    n = (0x100 * num) + num;
    ip += ":0"+n.toString(16);
    return ip;
}
function gpip(num) { return genip(num, prefix); }
function llip(num) { return genip(num, "fe80"); }
function genpref(pref){
    var ip = "";
    ip += pref;
    var n = pref.split(":");
    var r = 8 - n.length;
    for(var i=0; i<r; i++){
        ip +=":0000";
    }
    return ip;
}
function addroute(moteID, to, nexthop, len) {
    mote = sim.getMoteWithID(moteID);
    var cmd = "route -a "+gpip(to)+" "+llip(nexthop)+" "+len;
    log.log("MOTE " +moteID + "->" + cmd + "\n");
    write(mote, cmd);
}


//Waiting configurate of all mote was done
function waitingConfig(){
	var lastid = 0;
	for(var i=0;; i++) {
	    YIELD_THEN_WAIT_UNTIL(msg.contains("#s") || msg.contains("timeout"));
        if(msg.contains("#s")){
	        GENERATE_MSG(75000, "timeout"+lastid);
	        lastid++;
	    }else if(msg.equals("timeout"+(lastid-1))) {
	        return;
	    }
	}
}

//Send UDP packet from - to (number of mote)
function sendudp(from, to) {
    mote = sim.getMoteWithID(from);
    var cmd = "sendudp " + gpip(to);
    log.log("mote "+from+" -> "+cmd+"\n");
    write(mote, cmd);
    GENERATE_MSG(5000, "timeout"); //fail when timout (5s)
    YIELD_THEN_WAIT_UNTIL(msg.contains("DATA recv 'Hello") || msg.contains("timeout"));
    if(msg.contains("timeout")) {
        log.testFailed();
    }   
}

//Send UDP packet to the border router
function sendudpBR(br) {
    var allm = sim.getMotes();
    for(var i in  allm) {
        var id = allm[i].getID();
        if(id != br) {
            sendudp(id, br);
            sendudp(br, id);
        }
    }
}

//Build routing table when NA is received 
function buildRT(s) {
    while(true) {
	    YIELD_THEN_WAIT_UNTIL(msg.contains("#rNA"));
	    var from = msg.split(" ")[1].split("::")[1].split(":")[2];
	    var alldone = true;
	    for(var i in s){
	        var v = s[i];
	        if(v.mote == from) {
	            v.fct();
	            v.done = true;    
	        }
	        if(!v.done) alldone = false;
	    }
	    if(alldone) return;
    }
}
// endload(lib.js)


function addCO(prefix, len) {
    var cmd = "cp -a "+genpref(prefix)+" "+len;
    log.log("CO added -> "+cmd+"\n");
    write(sim.getMoteWithID(brID), cmd);
}

function rmCO(prefix, len) {
    var cmd = "cp -r "+genpref(prefix)+" "+len;
    log.log("CO remove -> "+cmd+"\n");
    write(sim.getMoteWithID(brID), cmd);
}


function checkTable(prefixes){
    var allm = sim.getMotes();
    for(var id in  allm) {
	    write(allm[id], "netd cp");
        do{
            YIELD();
            for(var i in prefixes) {
                var val = prefixes[i];
                if((msg.contains(val.pref) && val.ok) ||
                   (!msg.contains(val.pref) && !val.ok)) {
                    val.found = true;
                 }  
            }
        }while(!msg.contains("Contiki>"));
	}
    for(var i in prefixes) {
        if(!prefixes[i].found)
            return false;
    }
    return true;
}


/*---------------------------------------------------------------*/
TIMEOUT(7200000); //2h
WaitingStarting();

log.log("Modify RT\n");
buildRT([
    {"mote":2, 
     "fct":function(){
         addroute(1,3,2,128);
         //Waiting .5 sec
         GENERATE_MSG(500, "continue");
         WAIT_UNTIL(msg.contains("continue"));
         addroute(1,5,2,128);
        }
    },
    {"mote":4, 
     "fct":function(){
         addroute(1,6,4,128);
         }
    }
]);

waitingConfig();
log.log("Topology stable\n");
//displayAllTable();

if(!checkTable([{"pref":prefix+"::/16", "ok":1}])){
    log.testFailed();
}
log.log("Context found\n");

addCO(newprefix,16);
rmCO(prefix,16);
log.log("Modify Context Table\n");

function refreshTopo(){
	YIELD_THEN_WAIT_UNTIL(msg.contains("#sRS"));
	waitingConfig();
	displayAllTable();
	log.log("Topolgy refreshed\n");
}

refreshTopo();
if(!checkTable([{"pref":prefix+"::/16", "ok":1},
                {"pref":newprefix+"::/16", "ok":1}])) {
    log.testFailed();
}

refreshTopo();
if(!checkTable([{"pref":prefix+"::/16", "ok":0},
                {"pref":newprefix+"::/16", "ok":1}])) {
    log.testFailed();
}

log.testOK();