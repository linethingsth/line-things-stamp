var  on = false;

// Change this to your generated service UUID
var userServiceUUID = "d13ac813-2082-4029-989f-02b0273ee029";
var writeCharacteristicUUID = "E9062E71-9E62-4BC6-B0D3-35CDCD9B027B";
var notifyCharacteristicUUID = "62FBD229-6EDD-4D1A-B554-5C4E1BB29169";

var psdiServiceUUID = "E625601E-9E55-4597-A598-76018A0D293D";
var psdiCharacteristicUUID = "26E2B12B-85F0-4F3F-9FDD-91D114270E6E";

var deviceServices = {};
var userService = {};
var psdiService = {};
var userCharacteristics = {};
var psdiCharacteristics = {};

var updateButtonON = {};
var updateButtonOFF = {};

var buttonOnCharacteristic = {};
var buttonOffCharacteristic = {};

var deviceConnected = false;


NRF.on("disconnect", function() {
  deviceConnected = false;
  LED1.reset();
  clearInterval();
  setInterval(function() {
    on = !on;
    LED1.write(on);
    }, 300);
});

NRF.on("connect", function() {
  deviceConnected = true;
  clearInterval();
  LED1.set();
});

userCharacteristics[notifyCharacteristicUUID] = {
  value : 0,
  notify : true
};

userCharacteristics[writeCharacteristicUUID] = {
  value : 30,
  writable : true,
  onWrite : function(evt) {
    if(parseInt(evt.data) == 1){
      LED1.set();
    }
    else if(parseInt(evt.data) === 0){
      LED1.reset();
    }
  }
};

psdiCharacteristics[psdiCharacteristicUUID] = {
  value : "0123456789",
  readable : true
};

deviceServices[userServiceUUID] = userCharacteristics;
deviceServices[psdiServiceUUID] = psdiCharacteristics;

buttonOnCharacteristic[notifyCharacteristicUUID] = {
  value : 1,
  notify : true
};
buttonOffCharacteristic[notifyCharacteristicUUID] = {
  value : 0,
  notify : true
};

updateButtonON[userServiceUUID] = buttonOnCharacteristic;
updateButtonOFF[userServiceUUID] = buttonOffCharacteristic;

E.on('init', function() {
  NRF.setServices(deviceServices,{ uart : false, advertise : [ userServiceUUID ]});
  setInterval(function() {
  on = !on;
  LED1.write(on);
  }, 100);
});

setWatch(function() {
  // console.log("Button Pressed");
  NRF.updateServices(updateButtonON);
  LED1.set();
}, BTN, {edge:"rising", debounce:50, repeat:true});

setWatch(function() {
  // console.log("Button Released");
  NRF.updateServices(updateButtonOFF);
  LED1.reset();
}, BTN, {edge:"falling", debounce:50, repeat:true});
