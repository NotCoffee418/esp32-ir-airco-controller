//todo: 
// - cover screws instead of sketchy glue? clips to receive screws?
// - usb holder clips
// - space on inverse backplate (so it's not ultra tight)
// - ir hole
// - ir inner cover
// - mode switch hole
// - temperature sensor hole
// - against my better judgement, LCD for temp/status? https://www.gotron.be/0-96-oled-display-met-i2c-voor-arduino.html 


// variable to toggle between printable rotations and visual representation for braining
IS_PRINT_VIEW = false;

// beginning of board rest levels relative to case floor
board_rest_floor_diff = 20;


// USB slot hole size
usbc_slot_width = 9.88;
usbc_slot_height = 4.1;

// Screw hole size (for 6.5mm x 2.2mm with head 1.5)
screw_hole_depth = 6.5;
screw_head_depth = 1.8;
screw_head_radius = 4.4 / 2; // with 0.4 spacing
screw_hole_radius = 1.9 / 2;


// Mount block sizes
screw_mount_width = 10;
screw_mount_height = 10;






// Case Shell without right side
if (IS_PRINT_VIEW) {
	// Outer case without floor and back
	outer_case_without_floor_and_back_side();
	
	// Floor
	translate([120, 50, 0]) {
		floor();
	}	
} else {
	// Outer case without floor and back
	translate([100, 0, 100]) {
		rotate([0, 90, 90]) { 
			outer_case_without_floor_and_back_side();
		}
	}

	// Floor
	translate([3, 3, 0]) {
		floor();
	}	
}


// // Device holder slider bottom
// translate([20, 0, 10]) {
// 	rotate([180, 0, 0]) {
// 		device_holder_slider_bottom(110, -30);
// 	}
// }

// // Device holder unit left
// translate([30, 20, 30]) {
// 	rotate([90, 0, 0]) {
// 		device_holder_unit(140, -30, 0, true);
// 	}
// }

// // Device holder unit right
// translate([30, 0, -18]) {
// 	rotate([270, 0, 0]) {
// 		device_holder_unit(80, -30, 0, false);
// 	}
// }

// // Back panel
// outer_case_back_side(230,0,0); // mind the rounded side!
// rotate([0, 0, 90]) {
// 	back_panel_cover(120, -300, 0);
// }



module floor() {
	cube([94,94,3]);

	// Mount blocks sides (closer to front)
	translate([94, 0, 3]) {
		rotate([0, 0, 90]) {
			union() {
				// mount block left side
				translate([10, 0, 0]) {
					screw_mount_block(3 - screw_head_depth);
				}

				// mount block right side
				translate([20, 94, 0]) {
					rotate([0, 0, 180]) {
						// mount block left back
							screw_mount_block(3- screw_head_depth);
					}
				}
			}
		}
	}

	// Mount blocks sides (closer to back)
	translate([94, 64, 3]) {
		rotate([0, 0, 90]) {
			union() {
				// mount block left side
				translate([10, 0, 0]) {
					screw_mount_block(3 - screw_head_depth);
				}

				// mount block right side
				translate([20, 94, 0]) {
					rotate([0, 0, 180]) {
						// mount block left back
							screw_mount_block(3- screw_head_depth);
					}
				}
			}
		}
	}

	// Mount blocks back side
	translate([94, 94, 3]) {
		rotate([0, 0, 180]) {
			union() {
				// mount block left side
				translate([10, 0, 0]) {
					screw_mount_block(3 - screw_head_depth);
				}

				// mount block left side
				translate([74, 0, 0]) {
					screw_mount_block(3 - screw_head_depth);
				}
			}
		}
	}
}







// 8 wide
// with_difference_hole: 
// true: simulates a screw (used to difference in case)
// false: actual hole as visible
module backplate_screw_hole_holder(x, y, z, with_difference_hole=true) {
	translate([x,y+5,z]) {
		linear_extrude(height=1.5) { 
			difference() {
				union() {
					square([8, 5.5]); // Attahment clip square part
					translate([4, 0, 0]) // Attachment clip round part
						circle(r=4, $fn=100);

				}
				if (with_difference_hole) {
					translate([4, 0, 0])
						circle(r=screw_hole_radius, $fn=50); // Screw hole
				}
			}
			
		}
		if (!with_difference_hole) {
			translate([4, 0, 0]) {
				linear_extrude(height=3) {
					circle(r=screw_hole_radius, $fn=50); // Screw hole
				}
			}
		}
	}
}


// is_true_panel: true: actual panel, false: difference hole
module back_panel_cover(x, y, z, is_true_panel=true) {
	translate([x, y, z]) {
		difference() {
			// Back cover panel
			union() {
				rounded_square_wall([0, 0, 0], [80, 50,1.5], 1.5); // bot layer
				rounded_square_wall([3, 3, 1.5], [74, 44,1.5], 1.5); // top layer
				backplate_screw_hole_holder(10, -10, 0, is_true_panel);
				backplate_screw_hole_holder(62, -10, 0, is_true_panel);
				translate([80, 60, 0]) {
					rotate([0, 0, 180]) {
						backplate_screw_hole_holder(10, 0, 0, is_true_panel);
						backplate_screw_hole_holder(62, 0, 0, is_true_panel);
					}
				}
			}
			// Air cents
			if (is_true_panel) {
				union() {
					translate([15,11.5,0]) {
						air_vent();
					}
					translate([15,22.5,0]) {
						air_vent();
					}
					translate([15,33.5,0]) {
						air_vent();
					}
				}
			}
		}		
	}
}


module outer_case_without_floor_and_back_side() {
	
	// Mount screw hole relative positions
	screw_hole_pos_and_rot = get_mount_screw_hole_pos_and_rot();
	shx = screw_hole_pos_and_rot[0][0];
	shy = screw_hole_pos_and_rot[0][1];
	shz = screw_hole_pos_and_rot[0][2];
	shrotx = screw_hole_pos_and_rot[1][0];
	shroty = screw_hole_pos_and_rot[1][1];
	shrotz = screw_hole_pos_and_rot[1][2];

	difference() {
		// Box boxy
		rounded_cube([100, 100, 100], 2);

		// Inner cavity 3mm wall thickness
		translate([3, 3, 3])
			cube([97, 94, 97]);

		// Screw holes right side
		translate([100-3-shx, shy, 3+10+shz]) {
			rotate([shrotx, shroty, shrotz]) {
				screw_hole(3);
			}
		}
		
	}

	
	
}


module outer_case_back_side(x, y, z) {
	// 3mm missing on each side except top side, yet part of the case
	translate([x, y, z]) {
		difference() {
			rounded_cube([100, 100, 100], 2);
			translate([0, 0, 3])             // 3mm wall thickness
				cube([100, 100, 100]);       // inner cavity (leave 3mm bottom and back)
			translate([0,0,0])				// Cut off left rounded wall
				cube([100, 3, 100]);
			translate([0,97,0])				// Cut off right rounded wall
				cube([100, 3, 100]);

			translate([0,0,97])				// Cut off top rounded wall
				cube([100, 100, 3]);
			translate([97,0,0])				// Cut off front rounded wall
				cube([3, 100, 100]);

			// Back cover slot
			translate([65,3+94-80-7,0]) {
				rotate([0, 0, 90]) {
					back_panel_cover(-0, 0, 0, false);
				}
			}

			// USB-C slot
			translate([3+94-board_rest_floor_diff + 4.1, 3+94/2-30/2 ,0]) {
				rotate([0, 0, 90]) {
					usbc_slot_with_holder();
				}
			}
		}
	}
}


// Bars attached to the floor of the case for sliding in the device holder
module device_holder_slider_bottom(x, y) {
	// Configuration
	// Board size + excess space on holder slot (2mm each side)
	// 28.3 + 2 + 2 = 32.3
	depth = 32.3;
	single_slot_depth = 12;
	slide_clearance = 0.05; // on this side of the slider, total is *2
	rounding_cutoff = 1;

	union() {
		// Left bar
		// X 0 to 7.5
		translate([x, y, 0]) {
			difference() {
				rounded_cube([7.5-slide_clearance, depth, 10], 0.2);
				translate([5-slide_clearance, -rounding_cutoff, 0-rounding_cutoff])
					rounded_cube([2.5+slide_clearance+rounding_cutoff, depth+rounding_cutoff*2, 5+slide_clearance+rounding_cutoff], 0.2);
			}
		}

		// Right bar
		// X 12.5 to 20
		translate([x+12.5+slide_clearance*2, y, 0]) {
			difference() {
				rounded_cube([7.5-slide_clearance, depth, 10], 0.2);
				translate([-rounding_cutoff, -rounding_cutoff , 0-rounding_cutoff])
					rounded_cube([2.5+slide_clearance+rounding_cutoff, depth+rounding_cutoff*2, 5+slide_clearance+rounding_cutoff], 0.2);
			}
		}

		// Center block
		block_size = depth-single_slot_depth*2;
		translate([x, y+single_slot_depth, 0]) {
			rounded_cube([20+slide_clearance, block_size, 10], 0.2);
		}
	}
}

// Device holder unit - loose prints or with support
// is_left_holder either left holder or right holder
module device_holder_unit(x, y, z, is_left_holder) {
	single_slot_depth = 12;
	slide_clearance = 0.05;
	rounding_cutoff = 1;


	union() {
		// Bottom slide block
		translate([x, y, z]) {
			rounded_cube([10, single_slot_depth, 5-slide_clearance], 0.2);
		}
		// Middle slide block
		translate([x+2.5+slide_clearance, y, z+5-slide_clearance-1]) {
			rounded_cube([5-slide_clearance, single_slot_depth, 5 + slide_clearance], 0.2); // Arbitrary height for slide access without breaking risk
		}

		// Total height for slide system without beams is now 15mm from case floor

		// Holder support beam 3mm  on either side of device free
		if (is_left_holder) {
			translate([x+2.5+slide_clearance,y,z]) {
				difference() {
					rounded_cube([5-slide_clearance, 5, board_rest_floor_diff+5], 0.2);
					translate([-rounding_cutoff, 3-rounding_cutoff, board_rest_floor_diff]) {
						rounded_cube([3+rounding_cutoff, 3+rounding_cutoff, 2], 0.2);
					}
				}
			}
		} else {
			translate([x+2.5+slide_clearance,y+single_slot_depth-5,z]) {
				difference() {
					rounded_cube([5-slide_clearance, 5, board_rest_floor_diff+5], 0.2);
					translate([-rounding_cutoff, -rounding_cutoff, board_rest_floor_diff]) {
						rounded_cube([3+rounding_cutoff, 3+rounding_cutoff, 2], 0.2);
					}
				}
			}
		}
	}
}


module air_vent() {
	translate([0,0,-2]) {
		rounded_cube([50, 5, 7], 2);
	}
}

// 9.88mm x 4.1mm USB-C port hole - sticks 1.5mm out of board
module usbc_slot() {
    linear_extrude(height=3) {
        hull() {
            translate([1.5, 1.5]) circle(r=1.5, $fn=32);
            translate([8.38, 1.5]) circle(r=1.5, $fn=32);
            translate([8.38, 2.6]) circle(r=1.5, $fn=32);
            translate([1.5, 2.6]) circle(r=1.5, $fn=32);
        }
    }
}

module usbc_slot_with_holder() {
    inside_layer_offset = 1.5;

	usbc_slot_inner_padding = 0.5;


    board_rest_width = 30;
    board_rest_height = 2;

    // Board Rest
    translate([0, board_rest_height + usbc_slot_height/2, inside_layer_offset]) {
        cube([board_rest_width, usbc_slot_height, inside_layer_offset]);
    }

	// USBC slot inner padding
	translate([(board_rest_width - usbc_slot_width)/2 - usbc_slot_inner_padding/2, 0 - usbc_slot_inner_padding/2, inside_layer_offset]) {
		cube([usbc_slot_width + usbc_slot_inner_padding, usbc_slot_height + usbc_slot_inner_padding, inside_layer_offset]);
	}

    // USB-C port hole
    translate([(board_rest_width - usbc_slot_width)/2, 0, 0]) {
        usbc_slot();
    }

	// Button padding right
	translate([board_rest_width/2+usbc_slot_width/2+1.7, 1, inside_layer_offset]) {
		cube([4, 4, inside_layer_offset]);
	}

	// Button padding left
	translate([board_rest_width/2-usbc_slot_width/2-1.7-4, 1, inside_layer_offset]) {
		cube([4, 4, inside_layer_offset]);
	}
}

function get_mount_screw_hole_pos_and_rot(wall_thickness=3) = 
	[
		[screw_mount_width/2-1, wall_thickness, screw_mount_height/2],
		[90,90,0]
	];


// Plain screw hole, definable depth
module screw_hole(depth) {
	linear_extrude(height=depth) {
		circle(r=screw_hole_radius, $fn=20);
	}
}

// Screw head hole for outer wall (entrance to screw)
module screw_head_hole(wall_thickness=3) {
	translate([0, 0, wall_thickness-screw_head_depth]) {
		// Head hole
		linear_extrude(height=screw_head_depth) {
			circle(r=screw_head_radius, $fn=100);
		}

	}
	// wall hole
	screw_hole(wall_thickness - screw_head_depth);
}

// screw_head_offset distance claimed by case or other parts
module screw_mount_block(wall_thickness=3) {
	rounding_cutoff = 2;

	block_depth = screw_hole_depth - wall_thickness + rounding_cutoff + 5;
	translate([0, -rounding_cutoff, -rounding_cutoff]) {
		difference() {
			// Mount block
			rounded_cube([screw_mount_width, block_depth, screw_mount_width], rounding_cutoff);
			
			// Rounding cutoff on wall side
			translate([0, 0, 0]) {
				cube([screw_mount_width, rounding_cutoff, screw_mount_height]);
			}

			// Rounding cutoff on bottom for attachment
			translate([0, 0, 0]) {
				cube([screw_mount_width, block_depth, rounding_cutoff]);
			}
			
			
			// Screw hole in block
			translate([screw_mount_width/2, rounding_cutoff, screw_mount_height/2+rounding_cutoff/2]) {
				rotate([270, 0, 0]) {
					screw_hole(screw_hole_depth-wall_thickness);
				}
			}			
		}
	}
}


module rounded_cube(size, radius) {
    translate([radius, radius, radius])  // shift to correct position
        minkowski() {
            cube([size[0]-2*radius, size[1]-2*radius, size[2]-2*radius]);
            sphere(r=radius, $fn=50*radius);
        }
}

module rounded_square_wall(pos, size, radius) {
	translate([pos[0]+radius, pos[1]+radius, pos[2]])
		linear_extrude(height=size[2]) 
			minkowski() {
				square([size[0]-2*radius, size[1]-2*radius]);
			circle(r=radius, $fn=50);
		}
}
