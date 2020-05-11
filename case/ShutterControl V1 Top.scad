difference() {
	// Hauptcube
	cube([140,70, 31.4], center=true);

	// abgerundete Ecke vorne
	translate([0,-33,13.7]) {
		difference() {
				cube([142,4, 4], center=true);
				translate([0,2,-2]) {
					rotate([0,90,0]) {
						cylinder(144,d=8,$fn=40,center=true);
					}
				}
		}
	}

	// abgerundete Ecke hinten
	translate([0,33,13.7]) {
		rotate([0,0,180]) {
			difference() {
				cube([142,4, 4], center=true);
				translate([0,2,-2]) {
					rotate([0,90,0]) {
						cylinder(144,d=8,$fn=40,center=true);
					}
				}
			}
		}
	}

	// abgerundete Ecke rechts
	translate([68, 0, 13.7]) {
		rotate([0, 0, 90]) {
			difference() {
				cube([142, 4, 4], center=true);
				translate([0, 2, -2]) {
					rotate([0, 90, 0]) {
						cylinder(144, d=8, $fn=40, center=true);
					}
				}
			}
		}
	}

	// abgerundete Ecke links
	translate([-68, 0, 13.7]) {
		rotate([0, 0, -90]) {
			difference() {
				cube([142, 4, 4], center=true);
				translate([0, 2, -2]) {
					rotate([0, 90, 0]) {
						cylinder(144,d=8,$fn=40,center=true);
					}
				}
			}
		}
	}

	// Innenbox
	translate([0,0,-2.5]) {
		cube([135,65, 31.4], center=true);
	}

	// Aussparrung Deckel
	translate([0,0,-15.45]) {
		cube([137,68, 2.5], center=true);
	}

	// Aussparrung Ein/Aus-Schalter
	translate([68.5,15,-3]) {
		cube([5,13.25,8.25], center=true);
	}

	// Buttonloch
	for (x=[-38.5, -15.75, 7, 29.75 ]) {
		for (y=[-17, 3.32]) {
			translate([x, y, 14.2]) {
				cylinder(4,d=5,$fn=40,center=true);
			}
		}
	}

	// Buttonloch Level Select
	translate([52.5,-17, 14.2]) {
		cylinder(4,d=5,$fn=40,center=true);
	}
	
	// LEDs
	for (y=[-6, 3.25, 18.25]) {
		translate([52.5, y, 14.2]) {
			cylinder(4,d=3,$fn=40,center=true);
		}
	}
}


// linke und rechte Begrenzungen für die Platine
for (x=[61.25, -55.25 ]) {
	translate([x, 0, 9]) {
		cube([2.5, 67, 8], center=true);
	}
}

// Abstandshalter von oben zur Platinenoberseite
for (x=[-38.5, -15.75, 7, 29.75 ]) {
	for (y=[-22.25, -7, 8.25]) {
		translate([x, y, 10.7]) {
			cube([7.5,2.5,6.5], center=true);
		}
	}
}
for (y=[-22.25, -1.5]) {
	translate([52.5, y, 10.7]) {
		cube([7.5,2.5,6.5], center=true);
	}
}

// linke, obere Ecke
translate([-62.5,27.5,-5.45]) {
	difference() {
		cube([10,10, 17.5], center=true);
		translate([2.25,-2.25,6.25]) {
			rotate([0,-45,-45]) {
				cube([11,22, 22], center=true);
			}
		}
		cylinder(20,d=2.8,$fn=40,center=true);
		difference() {
			translate([2.5,-2.5,0]) {
				cube([5,5, 20], center=true);
			}
			cylinder(20,d=10,$fn=40,center=true);
		}
	}
}

// linke, untere Ecke
translate([-62.5,-27.5,-5.45]) {
	rotate([0,0,90]) {
		difference() {
			cube([10,10, 17.5], center=true);
			translate([2.25,-2.25,6.25]) {
				rotate([0,-45,-45]) {
					cube([11,22, 22], center=true);
				}
			}
			cylinder(20,d=2.8,$fn=40,center=true);
			difference() {
				translate([2.5,-2.5,0]) {
					cube([5,5, 20], center=true);
				}
				cylinder(20,d=10,$fn=40,center=true);
			}
		}
	}
}

// rechte, obere Ecke
translate([62.5,-27.5,-5.45]) {
	rotate([0,0,180]) {
		difference() {
			cube([10,10, 17.5], center=true);
			translate([2.25,-2.25,6.25]) {
				rotate([0,-45,-45]) {
					cube([11,22, 22], center=true);
				}
			}
			cylinder(20,d=2.8,$fn=40,center=true);
			difference() {
				translate([2.5,-2.5,0]) {
					cube([5,5, 20], center=true);
				}
				cylinder(20,d=10,$fn=40,center=true);
			}
		}
	}
}

// rechte, untere Ecke
translate([62.5,27.5,-5.45]) {
	rotate([0,0,270]) {
		difference() {
			cube([10,10, 17.5], center=true);
			translate([2.25,-2.25,6.25]) {
				rotate([0,-45,-45]) {
					cube([11,22, 22], center=true);
				}
			}
			cylinder(20,d=2.8,$fn=40,center=true);
			difference() {
				translate([2.5,-2.5,0]) {
					cube([5,5, 20], center=true);
				}
				cylinder(20,d=10,$fn=40,center=true);
			}
		}
	}
}
