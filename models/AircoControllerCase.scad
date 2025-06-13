// Case Shell without right side
translate([0, 0, 0])
union() {
	outer_case_without_left_side();
	device_holder_slider_bottom(110, -30);
	device_holder_unit(140, -30, 0, true);
	device_holder_unit(160, -30, 0, false);
}


module outer_case_without_left_side() {
	difference() {
		cube([100, 100, 100]);           // outer box
		translate([3, 3, 3])             // 3mm wall thickness
			cube([97, 94, 94]);          // inner cavity (leave 3mm bottom)
	}
}

// Bars attached to the floor of the case for sliding in the device holder
module device_holder_slider_bottom(x, y) {

	// Configuration
	// 28.3 + 3 + 3 = 34.3
	depth = 34.3;
	single_slot_depth = 15;
	slide_clearance = 0.05; // on this side of the slider, total is *2

	union() {
		// Left bar
		// X 0 to 7.5
		translate([x, y, 3]) {
			difference() {
				cube([7.5-slide_clearance, depth, 10]);  
				translate([5-slide_clearance, 0, 0])
					cube([2.5+slide_clearance, depth, 5+slide_clearance]);
			}
		}

		// Right bar
		// X 12.5 to 20
		translate([x+12.5+slide_clearance*2, y, 3]) {
			difference() {
				cube([7.5-slide_clearance, depth, 10]);  
				cube([2.5+slide_clearance, depth, 5+slide_clearance]);
			}
		}

		// Center block
		block_size = depth-single_slot_depth*2;
		translate([x, y+single_slot_depth, 3]) {
			cube([20+slide_clearance, block_size, 10]);  
		}
	}
}

// Device holder unit - loose prints or with support
// is_left_holder either left holder or right holder
module device_holder_unit(x, y, z, is_left_holder) {
	single_slot_depth = 15;
	slide_clearance = 0.05;
	device_grip_height = 30; // Bare minimum 20mm


	union() {
		// Bottom slide block
		translate([x, y, z]) {
			cube([10, single_slot_depth, 5-slide_clearance]);
		}
		// Middle slide block
		translate([x+2.5+slide_clearance, y, z+5-slide_clearance]) {
			cube([5-slide_clearance, single_slot_depth, 10 + slide_clearance]); // Arbitrary height for slide access without breaking risk
		}

		// Total height for slide system without beams is now 15mm from case floor

		// Holder support beam 3mm  on either side of device free
		if (is_left_holder) {
			translate([x+2.5,y,z+15]) {
				cube([5, 5, device_grip_height-15]);
			}
		} else {
			translate([x+2.5,y+single_slot_depth-5,z+15]) {
				cube([5, 5, device_grip_height-15]);
			}
		}
	}
}