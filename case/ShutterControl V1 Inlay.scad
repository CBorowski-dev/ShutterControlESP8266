high = 2;

translate([-46, -31, -0.75]) {
	difference() {
		cube([92, 63, high], center=false);

		translate([4, 11, 0]) {
			cube([22, 32, high], center=false);
		}

		translate([4, 47, 0]) {
			cube([22, 12, high], center=false);
		}

		translate([32, 11, 0]) {
			cube([4.5, 32, high], center=false);
		}

		translate([32, 47, 0]) {
			cube([4.5, 12, high], center=false);
		}

		translate([39, 11, 0]) {
			cube([7, 32, high], center=false);
		}

		translate([39, 47, 0]) {
			cube([7, 12, high], center=false);
		}

		translate([49, 11, 0]) {
			cube([40, 32, high], center=false);
		}

		translate([49, 47, 0]) {
			cube([40, 12, high], center=false);
		}
	}
}