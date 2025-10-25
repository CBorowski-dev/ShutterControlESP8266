translate([-68, -33, 0]) {
	difference() {
		cube([137.5, 67, 2.5]);

		// holes
		diameter = 3;
		translate([6, 6, 0]) {
			cylinder(20, d=diameter, $fn=40);
		}
		translate([131.5,6,0]) {
			cylinder(20, d=diameter, $fn=40);
		}
		translate([6,61,0]) {
			cylinder(20, d=diameter, $fn=40);
		}
		translate([131.5,61,0]) {
			cylinder(20, d=diameter, $fn=40);
		}
	}

	translate([13.5, 9, 2.5]) {
		cube([3, 49, 17]);
	}

	translate([75.5, 32, 2.5]) {
		cube([3, 26, 17]);
	}

	translate([100, 38, 2.5]) {
		cylinder(4,d=10,$fn=40);
	}
}