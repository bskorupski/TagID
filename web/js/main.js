var backEventListener = null;
var localMessagePort;
var nativeServiceAppId = "EUygfm8xpE.native"; // bridge app id (.wgt package)

var unregister = function() {
    if ( backEventListener !== null ) {
        document.removeEventListener( 'tizenhwkey', backEventListener );
        backEventListener = null;
        window.tizen.application.getCurrentApplication().exit();
    }
}

// Initialize function
var init = function () {
    // register once
    if ( backEventListener !== null ) {
        return;
    }
    
    // Initialization code
    console.log("init() called");
    
    // Launch context transmitter service
    tizen.application.launch(nativeServiceAppId, function(){
    	console.log('success');
    }, function(err){
    	console.log(err.message);
    });

    // Open message port
    localMessagePort = tizen.messageport.requestLocalMessagePort("RECEIVE_HELLO_MESSAGE");
    localMessagePort.addMessagePortListener(function(data, replyPort) {
    	$("#hello_display_area").text(data[0].value);
    });
    
    // Register click listener
	$("#send_btn").click(function() {
		var a = tizen.application.getAppInfo(nativeServiceAppId);
		
		// Open an message port to invoke message port 
		var remoteMessagePort = tizen.messageport.requestRemoteMessagePort(
				nativeServiceAppId, "RECEIVE_NAME");
		var name = $("#name").val();
		
		// Send a message
		remoteMessagePort.sendMessage([ {
			key : 'name',
			value : name
		} ], null);
	});
    
    var backEvent = function(e) {
        if ( e.keyName == "back" ) {
            try {
                if ( $.mobile.urlHistory.activeIndex <= 0 ) {
                    // if first page, terminate app
                    unregister();
                } else {
                    // move previous page
                    $.mobile.urlHistory.activeIndex -= 1;
                    $.mobile.urlHistory.clearForward();
                    window.history.back();
                }
            } catch( ex ) {
                unregister();
            }
        }
    }
    
    // Add eventListener for tizenhwkey (Back Button)
    document.addEventListener( 'tizenhwkey', backEvent );
    backEventListener = backEvent;
};

$(document).bind( 'pageinit', init );
$(document).unload( unregister );