extends Node2D

# Called when the node enters the scene tree for the first time.
func _ready():
	var s = Stigmark.new()
	print(s.login("hello", "world"))
	pass # Replace with function body.
