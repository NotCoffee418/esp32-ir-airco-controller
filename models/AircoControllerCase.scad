// Case Shell without right side
translate([0, 0, 0])
union() {
	outer_case_without_left_and_back_side(0,0,0);
	outer_case_left_and_back_side(150,0,0);
	device_holder_slider_bottom(110, -30);
	device_holder_unit(140, -30, 0, true);
	device_holder_unit(160, -30, 0, false);
}

module rounded_cube(size, radius) {
    translate([radius, radius, radius])  // shift to correct position
        minkowski() {
            cube([size[0]-2*radius, size[1]-2*radius, size[2]-2*radius]);
            sphere(r=radius, $fn=50*radius);
        }
}

module outer_case_without_left_and_back_side(x, y, z) {
	translate([x, y, z]) {
		difference() {
			rounded_cube([100, 100, 100], 2);
			translate([3, 3, 3])             // 3mm wall thickness
				cube([97, 94, 97]);          // inner cavity (leave 3mm bottom)
		}
	}
}

module outer_case_left_and_back_side(x, y, z) {
	translate([x, y, z]) {
		difference() {
			rounded_cube([100, 100, 100], 2);
			translate([3, 0, 3])             // 3mm wall thickness
				cube([100, 100, 100]);       // inner cavity (leave 3mm bottom and back)
			translate([0,0,0])				// Cut off left rounded wall
				cube([100, 3, 100]);
			translate([0,97,0])				// Cut off right rounded wall
				cube([100, 3, 100]);

			translate([0,0,97])				// Cut off top rounded wall
				cube([100, 100, 3]);
			translate([97,0,0])				// Cut off front rounded wall
				cube([3, 100, 100]);
		}
	}
}


// Bars attached to the floor of the case for sliding in the device holder
module device_holder_slider_bottom(x, y) {
	// Configuration
	// 28.3 + 3 + 3 = 34.3
	depth = 34.3;
	single_slot_depth = 15;
	slide_clearance = 0.05; // on this side of the slider, total is *2
	rounding_cutoff = 1;

	union() {
		// Left bar
		// X 0 to 7.5
		translate([x, y, 3]) {
			difference() {
				rounded_cube([7.5-slide_clearance, depth, 10], 0.2);
				translate([5-slide_clearance, 0-rounding_cutoff, 0-rounding_cutoff])
					rounded_cube([2.5+slide_clearance+rounding_cutoff, depth+rounding_cutoff, 5+slide_clearance+rounding_cutoff], 0.2);
			}
		}

		// Right bar
		// X 12.5 to 20
		translate([x+12.5+slide_clearance*2, y, 3]) {
			difference() {
				rounded_cube([7.5-slide_clearance, depth, 10], 0.2);
				translate([-rounding_cutoff,-rounding_cutoff,-rounding_cutoff])
					rounded_cube([2.5+slide_clearance+rounding_cutoff, depth+rounding_cutoff, 5+slide_clearance+rounding_cutoff], 0.2);
			}
		}

		// Center block
		block_size = depth-single_slot_depth*2;
		translate([x, y+single_slot_depth, 3]) {
			rounded_cube([20+slide_clearance, block_size, 10], 0.2);
		}
	}
}

// Device holder unit - loose prints or with support
// is_left_holder either left holder or right holder
module device_holder_unit(x, y, z, is_left_holder) {
	single_slot_depth = 15;
	slide_clearance = 0.05;
	device_grip_height = 30; // Bare minimum 20mm
	rounding_cutoff = 1;


	union() {
		// Bottom slide block
		translate([x, y, z]) {
			rounded_cube([10, single_slot_depth, 5-slide_clearance], 0.2);
		}
		// Middle slide block
		translate([x+2.5+slide_clearance, y, z+5-slide_clearance-1]) {
			rounded_cube([5-slide_clearance, single_slot_depth, 10 + slide_clearance + 1], 0.2); // Arbitrary height for slide access without breaking risk
		}

		// Total height for slide system without beams is now 15mm from case floor

		// Holder support beam 3mm  on either side of device free
		if (is_left_holder) {
			translate([x+2.5+slide_clearance,y,z]) {
				difference() {
					rounded_cube([5-slide_clearance, 5, device_grip_height+5], 0.2);
					translate([-rounding_cutoff, 3-rounding_cutoff, device_grip_height]) {
						rounded_cube([3+rounding_cutoff, 3+rounding_cutoff, 2], 0.2);
					}
				}
			}
		} else {
			translate([x+2.5+slide_clearance,y+single_slot_depth-5,z]) {
				difference() {
					rounded_cube([5, 5, device_grip_height+5], 0.2);
					translate([-rounding_cutoff, -rounding_cutoff, device_grip_height]) {
						rounded_cube([3+rounding_cutoff, 3+rounding_cutoff, 2], 0.2);
					}
				}
			}
		}
	}
}