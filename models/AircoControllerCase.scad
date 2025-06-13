// Case Shell without right side
translate([0, 0, 0])
union() {
	outer_case_without_left_side();
	device_holder_slider_bottom(110, -30, 0);
}


module outer_case_without_left_side() {
	difference() {
		cube([100, 100, 100]);           // outer box
		translate([3, 3, 3])             // 3mm wall thickness
			cube([97, 94, 94]);          // inner cavity (leave 3mm bottom)
	}
}

// Bars attached to the floor of the case for sliding in the device holder
module device_holder_slider_bottom(x, y, rotation) {
	// Configuration
	// 28.3 + 3 + 3 = 34.3
    depth = 34.3;
	single_slot_depth = 15;
	slide_clearance = 0.05; // on this side of the slider, total is *2
	
	// Left bar
	// X 0 to 7.5
	translate([x, y, 3]) rotate([0, 0, rotation]) {
		difference() {
			cube([7.5-slide_clearance, depth, 10]);  
			translate([5-slide_clearance, 0, 0])
				cube([2.5+slide_clearance, depth, 5+slide_clearance]);
		}
	}

	// Right bar
	// X 12.5 to 20
	translate([x+12.5+slide_clearance*2, y, 3]) rotate([0, 0, rotation]) {
		difference() {
			cube([7.5-slide_clearance, depth, 10]);  
			cube([2.5+slide_clearance, depth, 5+slide_clearance]);
		}
	}

	// Center block
	block_size = depth-single_slot_depth*2;
	translate([x, y+single_slot_depth, 3]) rotate([0, 0, rotation]) {
		cube([20+slide_clearance, block_size, 10]);  
	}
}