<!DOCTYPE html>

<html lang="en">

<head>

	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />

	<link rel="stylesheet" href="bootstrap.min.css" type="text/css" media="screen">
	<link rel="stylesheet" href="all.min.css" type="text/css" media="screen">
	<link rel="stylesheet" href="main.css" type="text/css" media="screen">

	<script type="text/javascript" src="jquery-3.4.1.min.js"></script>
	<script type="text/javascript" src="bootstrap.min.js"></script>
	<script type="text/javascript" src="main.js"></script>

	<title>Led strip PWM control</title>

</head>

<body>

	<nav class="navbar navbar-expand-md navbar-dark fixed-top bg-dark">
		<a class="navbar-brand" href="#">Mood Lamp</a>
		<button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarCollapse"
			aria-controls="navbarCollapse" aria-expanded="false" aria-label="Toggle navigation">
			<span class="navbar-toggler-icon"></span>
		</button>
		<div class="collapse navbar-collapse" id="navbarCollapse">
			<ul class="navbar-nav mr-auto">
				<li class="nav-item active">
					<a class="nav-link" href="#">Main<span class="sr-only">(current)</span></a>
				</li>
				<li class="nav-item">
					<a class="nav-link" href="#">Colors</a>
				</li>
				<li class="nav-item">
					<a class="nav-link" href="#">Settings</a>
				</li>
			</ul>
		</div>
	</nav>

	<div
		style="margin: 6em 0 0 0">

		<div class="card-deck">
			<div class="card">
				<div class="card-body mode off-mode">
					<i class="fas fa-power-off"></i>
				</div>
			</div>
			<div class="card">
				<div class="card-body mode color-mode">
					<i class="fas fa-tint"></i>
				</div>
			</div>
			<div class="card">
				<div class="card-body mode mood-mode">
					<i class="fas fa-sun"></i>
				</div>
			</div>
		</div>

	</div>

	<div
		id="color-modes">

		<div 
			data-red="255"
			data-green="255"
			data-blue="255"
			style="background: #eef"
			class="color-box">

		</div>

		<div 
			data-red="255"
			data-green="160"
			data-blue="0"
			style="background: rgb(255,160,0)"
			class="color-box">

		</div>

	</div>


	<div id="info-box-error" class="info-box error">
		<span class="text"></span>
	</div>

	<div id="info-box-warning" class="info-box warning">
		<span class="text"></span>
	</div>

	<div id="info-box-info" class="info-box info">
		<span class="text"></span>
	</div>

	<nav class="nav nav-pills nav-justified">
		<a class="nav-item nav-link active" data-toggle="pill" id="nav-mode-setting" role="tab"
			aria-controls="pills-mode-setting" aria-selected="true" href="#mode-setting">Mode setting</a>
		<a class="nav-item nav-link" data-toggle="pill" id="nav-color-ramps" role="tab"
			aria-controls="pills-color-ramps" aria-selected="true" href="#color-ramps">Color ramps</a>
		<a class="nav-item nav-link" data-toggle="pill" id="nav-mood-lamp" role="tab" aria-controls="pills-mood-lamp"
			aria-selected="true" href="#mood-lamp">Mood lamp</a>
	</nav>

	<div id="div-tab-content" class="tab-content">

		<div class="tab-pane fade show active" role="tabpanel" aria-labelledby="pills-mode-setting" id="mode-setting">

			<div class="radio-mode" style="background-color: #333" data-value="0">
				<i class="fas fa-power-off"></i><span>Off</span>
			</div>

			<div class="radio-mode" style="background-color: #e00" data-value="1">
				<span>Static day light</span>
			</div>

			<div class="radio-mode" style="background-color: #ee0" data-value="2">
				<span>Static night light</span>
			</div>

			<div class="radio-mode" style="background-color: #0e0" data-value="3">
				<span>Free cycling RGB light</span>
			</div>

			<div class="radio-mode" style="background-color: #00e" data-value="4">
				<span>Mood lamp</span>
			</div>

			<div class="radio-mode" style="background-color: #ea0" data-value="5">
				<span>Fade to Mood lamp</span>
			</div>

		</div>


		<div class="tab-pane fade" role="tabpanel" aria-labelledby="pills-home-tab" id="color-ramps">

			<ul class="list-group">

				<li class="list-group-item d-flex justify-content-between align-items-center">
					<div class="colorgauge">

						<span id="input-red-value" data-value="%red_value%" class="fill red">

						</span>

					</div>
				</li>

				<li class="list-group-item">

					<div class="colorgauge">
						<span id="input-green-value" data-value="%green_value%" class="fill green">
						</span>

					</div>


				</li>

				<li class="list-group-item">

					<div class="colorgauge">
						<span id="input-blue-value" data-value="%blue_value%" class="fill blue">
						</span>

					</div>

				</li>

			</ul>

		</div>

		<div class="tab-pane fade" role="tabpanel" aria-labelledby="pills-home-tab" id="mood-lamp">

			<div class="row justify-content-md-center">

				<div class="col-6">

					<div class="mood-setting">

						<span>Fade-in start:</span>
						<input type="time" id="mood-in-start" class="time-input" data-orig-sec="%mood_in_start%"
							value="07:00" />

						<span>Fade-in end:</span>
						<input type="time" class="time-input" id="mood-in-end" data-orig-sec="%mood_in_end%"
							value="07:15" />
					</div>

					<div class="mood-setting">

						<span>Fade-out start:</span>
						<input type="time" class="time-input" id="mood-out-start" data-orig-sec="%mood_out_start%"
							value="07:45" />

						<span>Fade-out end:</span>
						<input type="time" class="time-input" id="mood-out-end" data-orig-sec="%mood_out_end%"
							value="08:00" />

					</div>

					<div style="text-align: right">

						<button type="button" class="btn btn-primary" id="set-mood-button">
							Set times
						</button>

					</div>

				</div>

			</div>

		</div>

	</div>

</body>

</html>