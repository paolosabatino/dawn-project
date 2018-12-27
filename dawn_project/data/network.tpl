<!DOCTYPE html>

<head>
	
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta name="viewport" content="width=device-width, initial-scale=1">
	
	<link rel="stylesheet" href="main.css" type="text/css" media="screen">

	<title>Led strip PWM control - Network configuration</title>

</head>

<body>
	
	<div
		id="wrapper">
		
		<form
			action="nwset"
			method="GET"
			enctype="multipart/form-data">
			
			<div
				class="network-data">
				
				<label
					for="network-ssid">Network SSID:</label>
				<input
					type="text"
					id="network-ssid"
					name="network_ssid"
					value="%ssid%" />
					
			</div>
			
			<div
				class="network-data">
				<label
					for="network-passwd">Passphrase:</label>
					
				<input
					type="text"
					id="network-passwd"
					name="network_passwd"
					value="%passwd%" />
					
			</div>
			
			<div
				style="margin: 2em 0">
				<button
					type="submit">Send</button>
			</div>
			
		</form>
			
	</div>

</body>

</html>
