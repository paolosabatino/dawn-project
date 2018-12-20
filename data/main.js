var foreach = function(domElements, func) {
	
	var idx;
	
	for (idx = 0; idx < domElements.length; idx++)
		func(domElements[idx]);
	
};

var elapseTimers = {
	'color': null,
	'mode': null,
	'mood-settings': null
};

var messageTimeoutHandle = null;

var showMessage = function(message, type, timeout) {
	
	var dombox;
	var domspan;
	
	if (type === undefined)
		type = 'info';
		
	if (timeout === undefined)
		timeout = 3.0;
		
	switch (type) {
		case 'info':
		default:
			dombox = document.getElementById("info-box-info");
			break;
		case 'warning':
			dombox = document.getElementById("info-box-warning");
			break;
		case 'error':
			dombox = document.getElementById("info-box-error");
			break;
	}
	
	domspan = dombox.getElementsByClassName("text")[0];
	domspan.textContent = message;
	
	if (messageTimeoutHandle !== null) {
		
		var domboxes = document.getElementsByClassName("info-box");
		var idx;
		
		clearTimeout(messageTimeoutHandle);
		
		for (idx = 0; idx < domboxes.length; idx++)
			domboxes[idx].classList.remove("show");
		
	}
	
	dombox.classList.add("show");
	
	messageTimeoutHandle = setTimeout(function () {
		messageTimeoutHandle = null;
		dombox.classList.remove("show");
	}, timeout * 1000);
	
}

var updateFunctions = {};

updateFunctions['color'] = function () {
		
	var uri;
	
	var redValue = document.getElementById("input-red-value");
	var greenValue = document.getElementById("input-green-value");
	var blueValue = document.getElementById("input-blue-value");
	
	var args = {
		red: redValue.dataset['value'],
		green: greenValue.dataset['value'],
		blue: blueValue.dataset['value']
	};
	
	uri = "/set" + objToQueryString(args);
	
	console.log("request to:", uri);
	
	var oReq = new XMLHttpRequest();
	oReq.addEventListener("progress", ajaxlistener);
	oReq.addEventListener("load", ajaxlistener);
	oReq.addEventListener("error", ajaxlistener);
	oReq.addEventListener("abort", ajaxlistener);

	oReq.open("GET", uri);
	oReq.send();
	
	showMessage("processing", "info");
	
};

updateFunctions['mode'] = function () {
		
	var uri;
	
	var args = {
		mode: getRadioValue("radio_mode")
	};
	
	uri = "/mode" + objToQueryString(args);
	
	console.log("request to:", uri);
	
	var oReq = new XMLHttpRequest();
	oReq.addEventListener("progress", ajaxlistener);
	oReq.addEventListener("load", ajaxlistener);
	oReq.addEventListener("error", ajaxlistener);
	oReq.addEventListener("abort", ajaxlistener);
	
	console.log(uri);

	oReq.open("GET", uri);
	oReq.send();
	
	showMessage("processing", "info");
	
}

updateFunctions['mood_settings'] = function () {
	
	var uri;
	
	var args = {
		mood_in_start: timeToTimestamp(document.getElementById("mood-in-start").value),
		mood_in_end: timeToTimestamp(document.getElementById("mood-in-end").value),
		mood_out_start: timeToTimestamp(document.getElementById("mood-out-start").value),
		mood_out_end: timeToTimestamp(document.getElementById("mood-out-end").value)
	};
	
	uri = "/mood_settings" + objToQueryString(args);
	
	console.log("request to:", uri);
	
	var oReq = new XMLHttpRequest();
	oReq.addEventListener("progress", ajaxlistener);
	oReq.addEventListener("load", ajaxlistener);
	oReq.addEventListener("error", ajaxlistener);
	oReq.addEventListener("abort", ajaxlistener);

	oReq.open("GET", uri);
	oReq.send();
	
	showMessage("processing", "info");
	
};

var objToQueryString = function(obj) {
	
	var item;
	var parts = [];
	var qstring;
	
	for (item in obj)
		parts.push(item + "=" + obj[item]);
	
	
	qstring = "?" + parts.join("&");
	
	return qstring;
	
};

var getRadioValue = function() {
	
	var obj = document.getElementById("radio-mode-div");
	
	return obj.dataset['value'];
	
};

var addClass = function(domobj, className) {
	
	var classes = domobj.className.split(" ");
	
	if (classes.indexOf(className) == -1) {
		classes.push(className);
		domobj.className = classes.join(" ");
	}
	
};

var removeClass = function(domobj, className) {
	
	var index;
	var classes = domobj.className.split(" ");
	
	index = classes.indexOf(className);
	
	if (index != -1) {
		classes.splice(index, 1)
		domobj.className = classes;
	}
	
};

var hasClass = function(domobj, className) {
	
	return domobj.className.split(" ").indexOf(className) !== -1;
	
};

var validateTimeDomObject = function(domobj) {
	
	var idx;
	var parts;
	
	try {
		
		parts = domobj.value.split(":");
		
		if (parts.length < 2 || parts.length > 3)
			throw ("not enough parts");
			
		for (idx=0; idx<parts.length; idx++) {
			if (parts[idx] === "")
				throw ("part is empty");
		}					
		
		if (parts[0] < 0 || parts[0] > 23)
			throw ("invalid hour value");
			
		if (parts[1] < 0 || parts[1] > 59)
			throw ("invalid minute value");
			
		if (parts.length == 3 && (parts[2] < 0 || parts[2] > 59))
			throw ("invalid second value");
			
		removeClass(domobj, "error");
			
	} catch (e) {
		
		console.log("error in", domobj, ":", e);
		addClass(domobj, "error");
		
	}				
	
};

var timeToTimestamp = function(time) {
	
	var parts = time.split(":");
	var ts = 0;
	
	ts = (parts[0] >> 0) * 3600 + (parts[1] >> 0) * 60 + (parts[2] >> 0);
	
	return ts;
	
};

var timestampToTime = function(ts) {
	
	var hours = ((ts / 3600) >> 0).toString();
	var minutes = (((ts % 3600) / 60) >> 0).toString();
	var seconds = (ts % 60).toString();;
	
	if (minutes.length < 2)
		minutes = "0" + minutes;
		
	if (seconds.length < 2)
		seconds = "0" + seconds;
		
	return hours + ":" + minutes + ":" + seconds;
	
}
	

var ajaxlistener = function(evt) {
	
	switch (evt.type) {
		case 'error':
			showMessage("An error occurred " + evt.target.responseText, 'error', 5);
			break;
		case 'progress':
			break;
		case 'load':
			showMessage("Done", "info", 5);
			break;
		case 'abort':
			showMessage("Request aborted", "warning", 5);
			break;
		default:
			showMessage("Unkown event", "warning", 5);
			break;
	}
	
	console.log("AJAX:", evt);
	
};

var timeUpdate = function (what) {
	
	if (elapseTimers[what] !== null) {
		clearTimeout(elapseTimers[what]);
		elapseTimers[what] = null;
	}
	
	elapseTimers[what] = setTimeout (updateFunctions[what], 100);
	
};			

document.addEventListener("DOMContentLoaded", function(event) { 
	
	var gauges = document.getElementsByClassName("colorgauge");
	var radios = document.getElementsByClassName("radio-mode");
	var times = document.getElementsByClassName("time-input");
	var setMoodButton = document.getElementById("set-mood-button");
	
	var handleGauge = function(evt) {
		
		var target, fill;
		var x;
			
		target = evt.currentTarget;
		
		var boundings = target.getBoundingClientRect()
		if (evt.changedTouches === undefined) {
			x = (evt.clientX - boundings.x) / boundings.width;
		} else {
			if (evt.changedTouches.length > 0)
				x = (evt.changedTouches[0].clientX - boundings.x) / boundings.width;
		}
		 
		if (x < 0)
			x = 0;
		else if (x > 1)
			x = 1;
		
		fill = target.children[0];
		
		fill.style['width'] = (x * 100).toString() + "%";
		fill.dataset['value'] = (x * 255) >> 0;
		
	};
	
	foreach(gauges, function(gauge) {
		
		var fill = gauge.children[0];
		fill.style['width'] = ((fill.dataset['value'] / 255) * 100).toString() + "%";
	
		gauge.addEventListener("mousemove", function(evt) {
			
			if (evt.buttons != 1)
				return;
			
			handleGauge(evt);
			
			clearTimeout(elapseTimers['color']);
			elapseTimers['color'] = null;
			
		});
		
		gauge.addEventListener("touchmove", function(evt) {
			
			console.log(evt);
			
			handleGauge(evt);
			
			clearTimeout(elapseTimers['color']);
			elapseTimers['color'] = null;
			
		});
		
		gauge.addEventListener("touchend", function(evt) {
			
			console.log(evt);
			
			handleGauge(evt);
			timeUpdate('color');
			
		});
		
		gauge.addEventListener("mouseup", function(evt) {
			
			handleGauge(evt);
			timeUpdate('color');
			
		});

		gauge.addEventListener("mouseleave", function(evt) {
			
			if (evt.buttons != 1)
				return;
			
			handleGauge(evt);
			timeUpdate('color');
			
		});
		
		
	});
	
	foreach(radios, function(radio) {
		
		radio.addEventListener("click", function(evt) {
			
			var divMode = document.getElementById("radio-mode-div");
			divMode.dataset['value'] = evt.target.dataset['value']
			
			timeUpdate('mode');
			
		});
		
	});
	
	foreach(times, function(domobj) {
		
		var originSeconds = domobj.dataset['origSec'];
		var hours, minutes, seconds;
		
		if ((originSeconds >> 0).toString() == originSeconds)
			domobj.value = timestampToTime(originSeconds);
		
		domobj.addEventListener("onblur", function(evt) {
			
			validateTimeDomObject(evt.target);					
			
		});
		
		domobj.addEventListener("keypress", function(evt) {
			
			validateTimeDomObject(evt.target);					
			
		});
		
	});
	
	setMoodButton.addEventListener("click", function(evt) {
		
		var error = false;
		
		foreach (times, function(domobj) {
			
			validateTimeDomObject(domobj);
			error = error || hasClass(domobj, "error");
			
		});
		
		if (error === false)
			timeUpdate('mood_settings');
		
	});
	
});
