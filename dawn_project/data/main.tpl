<!DOCTYPE html>

<head>
	
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta name="viewport" content="width=device-width, initial-scale=1">
	
	<link rel="stylesheet" href="main.css" type="text/css" media="screen">
	<script type="text/javascript" src="main.js"></script>

	<title>Led strip PWM control</title>

</head>

<body>
	
	<div
		id="wrapper">
		
		<div
			id="info-box-error"
			class="info-box error">
			<span class="text"></span>
		</div>
		
		<div
			id="info-box-warning"
			class="info-box warning">
			<span class="text"></span>
		</div>
		
		<div
			id="info-box-info"
			class="info-box info">
			<span class="text"></span>
		</div>
		
		<div
			id="radio-mode-div"
			class="settings-panel"
			data-value="">
			<h2>Mode setting</h2>
			
			<div
				class="radio-mode"
				style="background-color: #333"
				data-value="0">
				<span>Off</span>
			</div>
			
			<div
				class="radio-mode"
				style="background-color: #e00"
				data-value="1">
				<span>Static day light</span>
			</div>
			
			<div
				class="radio-mode"
				style="background-color: #ee0"
				data-value="2">
				<span>Static night light</span>
			</div>
			
			<div
				class="radio-mode"
				style="background-color: #0e0"
				data-value="3">
				<span>Free cycling RGB light</span>
			</div>
			
			<div class="radio-mode"
				style="background-color: #00e"
				data-value="4">
				<span>Mood lamp</span>
			</div>
			
			<div class="radio-mode"
				style="background-color: #ea0"
				data-value="5">
				<span>Fade to Mood lamp</span>
			</div>
			
		</div>
		
		
		<div
			id="color-gauges-div"
			class="settings-panel">
			<h2>Color ramp settings</h2>
			
			<div
				class="cblock">
				
				<span
					class="label">Red color gauge:</span>
					
				<div
					class="colorgauge">
					<span
						id="input-red-value"
						data-value="%red_value%"
						class="fill red">
					</span>
					
				</div>
				
			</div>
				
			<div
				class="cblock">
				<span
					class="label">Green color gauge:</span>
				<div
					class="colorgauge">
					<span
						id="input-green-value"
						data-value="%green_value%"
						class="fill green">
					</span>
					
				</div>
			</div>
				
			<div
				class="cblock">
				<span
					class="label">Blue color gauge:</span>
				<div
					class="colorgauge">
					<span
						id="input-blue-value"
						data-value="%blue_value%"
						class="fill blue">
					</span>
					
				</div>
			</div>
			
		</div>
		
		<div
			id="mood-lamp-div"
			class="settings-panel">
			<h2>Mood lamp settings</h2>
			
			<div
				class="mood-setting">
				<span>Fade-in start:</span>
				<input
					type="text"
					id="mood-in-start" 
					class="time-input"
					data-orig-sec="%mood_in_start%"
					value="00:00" />
					
				<span>Fade-in end:</span>
				<input
					type="text"
					class="time-input"
					id="mood-in-end"
					data-orig-sec="%mood_in_end%"
					value="00:00" />
			</div>
			
			<div
				class="mood-setting">
				
				<span>Fade-out start:</span>
				<input
					type="text"
					class="time-input"
					id="mood-out-start"
					data-orig-sec="%mood_out_start%"
					value="00:00" />
					
				<span>Fade-out end:</span>
				<input
					type="text"
					class="time-input"
					id="mood-out-end"
					data-orig-sec="%mood_out_end%"
					value="00:00" />
				
			</div>
			
			<div
				style="overflow: hidden">
			
				<div
					style="float: right">
					
					<button
						id="set-mood-button">
						Set
					</button>
					
				</div>
			
			</div>
			
		</div>
	
		
	</div>

</body>

</html>
