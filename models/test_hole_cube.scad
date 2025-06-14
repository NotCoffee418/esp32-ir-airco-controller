
screw_hole_radius = 1.9 / 2;

difference() {
	rounded_cube([20,20,20],3);
	translate([10,10,0]) {
		screw_hole(20);
	}
	rotate([0, 90, 0]) {
		translate([-10,10,0]) {
			screw_hole(20);
		}
	}

	rotate([0, 90, 90]) {
		translate([-10,-10,0]) {
			screw_hole(20);
		}
	}	
}

module screw_hole(depth) {
	linear_extrude(height=depth) {
		circle(r=screw_hole_radius, $fn=20);
	}
}

module rounded_cube(size, radius) {
    translate([radius, radius, radius])  // shift to correct position
        minkowski() {
            cube([size[0]-2*radius, size[1]-2*radius, size[2]-2*radius]);
            sphere(r=radius, $fn=50*radius);
        }
}