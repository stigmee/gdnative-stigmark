extends Node2D

# Called when the node enters the scene tree for the first time.
func _ready():
	var s = Stigmark.new()
	var urls = s.search("mariadb")
	for url in urls:
		print(url)
	pass # Replace with function body.
