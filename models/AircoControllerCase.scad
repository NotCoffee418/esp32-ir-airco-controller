//todo: 
// - space on inverse backplate (so it's not ultra tight)
// - ir inner cover


// variable to toggle between printable rotations and visual representation for braining
IS_ASSEMBLED_VIEW = false;

// Disassembles back cover if in assembly view and true
DISASSEMBLE_BACK_COVER = false;

// beginning of board rest levels relative to case floor
board_rest_floor_diff = 20;

// Wall thickness (careful changing this)
wall_thickness = 4;

// USB slot hole size
usbc_slot_width = 9.88;
usbc_slot_height = 4.1;

// Screw hole size (for 6.5mm x 2.2mm with head 1.5)
screw_hole_depth = 7;
screw_head_depth = 1.8;
screw_head_radius = 4.5 / 2; // with 0.4 spacing



// 2.0: resin breaks at regular hole
// 2.05: resin breaks at backplate clip
// 2.1: resin tight fit
// 2.2: resin good
// presumable PLA holes shrink even further, but less breaky.
// Actual screw max diameter seems to be 2.10, and if holes and we can probably handle 0.2 too small in PLA(?)
screw_hole_radius = 2.1 / 2; // resin bre


// Mount block sizes
screw_mount_width = 10;
screw_mount_height = 10;

// 0 is valid, just leaves hole in the back and screw may poke out slighly.
// change freely but be careful of overlapping with other parts.
// 0 is good for checking if hole is positioned right and avoiding weird print issues at angles.
mount_block_back_padding = 0;

// Back cover
back_cover_width = 80;
back_cover_height = 50; // without clips but including cover spacing


// Board actual size
board_width = 28.4;
board_depth = 51.4; // without usbc overhang
board_height = 1.5;

// amount the USBC port sticks out of the board
usbc_port_overhang = 1.5;


// How deep does the device holder slide?
device_holder_single_slot_depth = 12;



// Case Shell without right side
if (!IS_ASSEMBLED_VIEW) {
	// Outer case without floor and back
	translate([0,100,0]) {
	rotate([180,270,0]) {
			outer_case_without_floor_and_back_side();
		}
	}

	// Floor
	translate([120, 50, 0]) {
		floor();
	}

	// case back side
	translate([230,0,0]) {
		outer_case_back_side(); // mind the rounded side!
	}

	// Case removable back cover
	translate([320, 120, 0]) {
		rotate([180, 0, 90]) {
			back_panel_cover();
		}
	}

	// Device holder unit right
	translate([110, 0-30, device_holder_single_slot_depth]) {
		rotate([270, 0, 0]) {
			device_holder_unit(false);
		}
	}

	// Device holder slider bottom
	translate([140, 0, 10]) {
		rotate([180, 0, 0]) {
			device_holder_slider_bottom();
		}
	}

	// Device holder unit left
	translate([180, 20-30, 0]) {
		rotate([90, 0, 0]) {
			device_holder_unit(true);
		}
	}

} else { // Assembled view
	//Outer case without floor and back
	translate([100, 0, 100]) {
		rotate([0, 90, 90]) { 
			outer_case_without_floor_and_back_side();
		}
	}

	// Floor
	translate([wall_thickness , wall_thickness, 0]) {
		floor();
	}

	// case back side
	translate([0,100,100]) {
		rotate([90, 90, 0]) {
			outer_case_back_side(); // mind the rounded side!
		}
	}

	// Case removable back cover
	if (DISASSEMBLE_BACK_COVER) {
		translate([200, 0, 0]) {
			rotate([0, 0, 90]) {
				back_panel_cover();
			}
		}
	} else {
		translate([6, 100, 35]) {
			rotate([90, 0, 0]) {
				back_panel_cover();
			}
		}
	}
	

	// Assembled device holder and spoof board
	translate([34.92,57.1,0]) {
		rotate([0,0,270]) {
			assembled_device_holder(true);
		}		
	}
}


//test_print_pieces();


module floor() {
	center = 100 / 2;

	// Distance from center of case to mount block
	// 0 should be center, 50 should just outside of case
	// this is reference for correct case holes.
	center_offset_pos = 25;

	// Floor
	difference() {
		// Floor block
		cube([100-wall_thickness*2,100-wall_thickness*2,wall_thickness]);

		// Screw hole device mount 1
		// lazy measure
		translate([
			47.07,
			56.6,
			wall_thickness]
		) {
			rotate([180,0,0]) {
				screw_head_hole();
			}
		}

		// Screw hole device mount 2
		// lazy measure
		translate([
			47.07,
			29.6,
			wall_thickness]
		) {
			rotate([180,0,0]) {
				screw_head_hole();
			}
		}		
	}

	

	// Mount block - right frontish
	translate([100-wall_thickness*2, center-center_offset_pos - screw_mount_width, wall_thickness]) {
		rotate([0, 0, 90]) {
			screw_mount_block();
		}
	}

	// Mount block - right side backish
	translate([100-wall_thickness*2, center+center_offset_pos-screw_mount_width+wall_thickness/2, wall_thickness]) {
		rotate([0, 0, 90]) {
			screw_mount_block();
		}
	}

	// Mount block - left side frontish
	translate([0, center - center_offset_pos, wall_thickness]) {
		rotate([0, 0, 270]) {
			screw_mount_block();
		}
	}


	// Mount block - left side backish
	translate([0, center + center_offset_pos + wall_thickness/2, wall_thickness]) {
		rotate([0, 0, 270]) {
			screw_mount_block();
		}
	}

	// Mount block - back side rightish
	translate([center-center_offset_pos, 100-wall_thickness*2, wall_thickness]) {
		rotate([0, 0, 180]) {
			screw_mount_block();
		}
	}

	// Mount block - back side left
	translate([center+center_offset_pos+wall_thickness/2, 100-wall_thickness*2, wall_thickness]) {
		rotate([0, 0, 180]) {
			screw_mount_block();
		}
	}

}



// 8 wide
// with_difference_hole: 
// true: simulates a screw (used to difference in case)
// false: actual hole as visible
module backplate_screw_hole_holder(x, y, z, with_difference_hole=true) {
	case_clip_width = 8;
	plate_clip_width = 7.6;
	clip_width = with_difference_hole ? plate_clip_width : case_clip_width;
	x_offset = !with_difference_hole ? 0 : (case_clip_width - plate_clip_width) / 2;

	translate([x+x_offset,y+5,z]) {
		linear_extrude(height=wall_thickness/2) { 
			difference() {
				union() {
					square([clip_width, 5.5]); // Attahment clip square part
					translate([clip_width/2, 0, 0]) // Attachment clip round part
						circle(r=clip_width/2, $fn=100);

				}
				if (with_difference_hole) {
					translate([clip_width/2, 0, 0])
						circle(r=screw_hole_radius, $fn=50); // Screw hole
				}
			}
			
		}
		if (!with_difference_hole) {
			translate([clip_width/2, 0, 0]) {
				linear_extrude(height=wall_thickness) {
					circle(r=screw_hole_radius, $fn=50); // Screw hole
				}
			}
		}
	}
}


// is_true_panel: true: actual panel, false: difference hole
module back_panel_cover(is_true_panel=true) {
	cover_slot_spacing = 3;
	corner_radius = 1.5;
	layer_height = wall_thickness/2;

	clearance_offset = 0.4;

	size_offset = is_true_panel ? clearance_offset : 0;
	
	translate([cover_slot_spacing+corner_radius/2,0,0]) { // dontk now why, dont care.
		difference() {
			// Back cover panel
			union() {
				// Outer Layer
				rounded_square_wall(
					[size_offset/2, size_offset/2, 0], 
					[back_cover_width-size_offset, back_cover_height-size_offset, layer_height], 
					corner_radius); // bot layer (bigger)

				// Inner Layer
				// Hide on true cover, but use for difference hole
				if (!is_true_panel) {
					rounded_square_wall(
						[cover_slot_spacing, cover_slot_spacing, layer_height],
						[back_cover_width-cover_slot_spacing*2, back_cover_height-cover_slot_spacing*2,layer_height], 
						corner_radius); // top layer
				}
				backplate_screw_hole_holder(10, -10, 0, is_true_panel);
				backplate_screw_hole_holder(62, -10, 0, is_true_panel);
				translate([80, 60, 0]) {
					rotate([0, 0, 180]) {
						backplate_screw_hole_holder(10, 0, 0, is_true_panel);
						backplate_screw_hole_holder(62, 0, 0, is_true_panel);
					}
				}
			}
			// Air vents
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
	screw_hole_pos_and_rot = get_mount_screw_head_hole_pos_and_rot();
	shx = screw_hole_pos_and_rot[0][0];
	shy = screw_hole_pos_and_rot[0][1];
	shz = screw_hole_pos_and_rot[0][2];
	shrotx = screw_hole_pos_and_rot[1][0];
	shroty = screw_hole_pos_and_rot[1][1];
	shrotz = screw_hole_pos_and_rot[1][2];

	center = 100 / 2;

	// why 1? no clue, but it works.
	screw_pos_center_offset = 25 + 1;

	difference() {
		// Box boxy
		rounded_cube([100, 100, 100], 2);

		// Inner cavity 3mm wall thickness
		translate([wall_thickness, wall_thickness, wall_thickness])
			cube([100-wall_thickness, 100-wall_thickness*2, 100-wall_thickness]);


		// Screw holes right side (front sided)
		translate([100-wall_thickness-shx, shy, center-screw_pos_center_offset]) {
			rotate([shrotx, shroty, shrotz]) {
				screw_head_hole();
			}
		}

		// Screw holes right side (back sided)
		translate([100-wall_thickness-shx, shy, center+screw_pos_center_offset]) {
			rotate([shrotx, shroty, shrotz]) {
				screw_head_hole();
			}
		}

		// Screw holes left side (front sided)
		translate([100-wall_thickness-shx, 100-shy, center-screw_pos_center_offset]) {
			rotate([shrotx+180, shroty, shrotz]) {
				screw_head_hole();
			}
		}

		// Screw holes left side (back sided)
		translate([100-wall_thickness-shx, 100-shy, center+screw_pos_center_offset]) {
			rotate([shrotx+180, shroty, shrotz]) {
				screw_head_hole();
			}
		}


		// Temperature sensor hole (can place wherever it seems.)
		translate([80,0,50]) {
			temperature_sensor_hole();
		}

		
		// IR hole
		//y -2.5+50
		translate([wall_thickness,50,80]) {
			rotate([0,270,0]) {
				ir_hole();
			}
		}

		// Mode switch hole
		translate([0,50,20]) {
			mode_switch_hole();
		}

	}

	
}


module outer_case_back_side() {
	// 3mm missing on each side except top side, yet part of the case
	difference() {
		rounded_cube([100, 100, 100], 2);

		// inner cavity (leave 3mm bottom and back)
		translate([0, 0, wall_thickness])
			cube([100, 100, 100]);       
			
		// Cut off left rounded wall
		translate([0,0,0])
			cube([100, wall_thickness, 100]);
			

		// Cut off right rounded wall
		translate([0,100-wall_thickness,0])
			cube([100, wall_thickness, 100]);


		// Cut off top rounded wall
		translate([0,0,0])
			cube([wall_thickness, 100, 100]);

		
		// Back cover slot
		translate([65,(100-wall_thickness*2-back_cover_width)/2,0]) {
			rotate([0, 0, 90]) {
				back_panel_cover(false);
			}
		}

		// USB-C slot
		translate([
			100-wall_thickness-board_rest_floor_diff + usbc_slot_height, 
			100/2-wall_thickness-usbc_slot_width,
			0]) {
			rotate([0, 0, 90]) {
				usbc_slot_with_holder();
			}
		}
	}

}


module assembled_device_holder(with_board=true) {
	// Device holder slider bottom
	translate([0, 0, wall_thickness]) {
		device_holder_slider_bottom();
	}

	// Device holder unit left
	translate([5, 0, wall_thickness]) {
		device_holder_unit(true);
	}

	// Device holder unit right
	translate([5, 20.3, wall_thickness]) {
		device_holder_unit(false);
	}

	if (with_board) {
		translate([10,2,wall_thickness+board_rest_floor_diff+0.1]) {		
			rotate([0,0,90]) {
				spoof_board();
			}
		}
	}
}

module spoof_board() {	
	usb_holder_slot_width = 30;
	excess_offset = usb_holder_slot_width - board_width;

	difference() {
		// Board
		union() {
			// Surface
			rounded_square_wall(
				[0, 0, 0], 
				[board_width, board_depth, board_height], 
				1.5);

			// Copy of USBc slot holder - dont mess with it to fix this, just mess with this arbitrary offset
			// theoretically it's done by the time this function exists so... should be ok?
			translate([
				-(excess_offset/2),
				board_depth+usbc_port_overhang,
				-4.1]
			) {
				rotate([90,0,0]) {
					usbc_slot_with_holder();
				}
			}
		}
		
		// Remove anything above the surface
		// 20 is arbitrary
		translate([-20,-20,board_height]) {
			cube([board_width+40, board_depth+40, 100]);
		}

		
		// Remove stuff on the right
		translate([-20,0,-20]) {
			cube([20, board_depth, 100]);
		}

		// Remove stuff on the left
		translate([board_width,-20,-20]) {
			cube([20, board_depth+40, 100]);
		}

	}
		
	
	
	
}


// Bars attached to the floor of the case for sliding in the device holder
module device_holder_slider_bottom() {
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
		difference() {
			rounded_cube([7.5-slide_clearance, depth, 10], 0.2);
			translate([5-slide_clearance, -rounding_cutoff, 0-rounding_cutoff])
				rounded_cube([2.5+slide_clearance+rounding_cutoff, depth+rounding_cutoff*2, 5+slide_clearance+rounding_cutoff], 0.2);
		}

		// Right bar
		// X 12.5 to 20
		translate([12.5+slide_clearance*2, 0, 0]) {
			difference() {
				rounded_cube([7.5-slide_clearance, depth, 10], 0.2);
				translate([-rounding_cutoff, -rounding_cutoff , 0-rounding_cutoff])
					rounded_cube([2.5+slide_clearance+rounding_cutoff, depth+rounding_cutoff*2, 5+slide_clearance+rounding_cutoff], 0.2);
			}
		}

		// Center block
		block_size = depth-single_slot_depth*2;
		translate([0, single_slot_depth, 0]) {
			rounded_cube([20+slide_clearance, block_size, 10], 0.2);
		}

		// Screw Block 1
		translate([
			rounding_cutoff, // slightly go into the bar
			depth/2+screw_mount_width/2, // center of the bar
			0]
		) {
			rotate([90,0,270]) {
				screw_mount_block(true);
			}
		}

		// Screw Block 2
		translate([
			20-rounding_cutoff,
			depth/2-(screw_mount_width/2),
			0]
		) {
			rotate([90,0,90]) {
				screw_mount_block(true);
			}
		}
	}
}

// Device holder unit - loose prints or with support
// is_left_holder either left holder or right holder
module device_holder_unit(is_left_holder) {
	slide_clearance = 0.05;
	rounding_cutoff = 1;


	union() {
		// Bottom slide block
		rounded_cube([10, device_holder_single_slot_depth, 5-slide_clearance], 0.2);
		
		// Middle slide block
		translate([2.5+slide_clearance, 0, 5-slide_clearance-1]) {
			rounded_cube([5-slide_clearance, device_holder_single_slot_depth, 6], 0.2); // Arbitrary height for slide access without breaking risk
		}

		// Total height for slide system without beams is now 15mm from case floor

		// Holder support beam 3mm  on either side of device free
		if (is_left_holder) {
			translate([2.5+slide_clearance,0,0]) {
				difference() {
					rounded_cube([5-slide_clearance, 5, board_rest_floor_diff+5], 0.2);
					translate([-rounding_cutoff, 3-rounding_cutoff, board_rest_floor_diff]) {
						rounded_cube([3+rounding_cutoff, 3+rounding_cutoff, 2], 0.2);
					}
				}
			}
		} else {
			translate([2.5+slide_clearance,device_holder_single_slot_depth-5,0]) {
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
    linear_extrude(height=wall_thickness) {
        hull() {
            translate([1.5, 1.5]) circle(r=1.5, $fn=32);
            translate([8.38, 1.5]) circle(r=1.5, $fn=32);
            translate([8.38, 2.6]) circle(r=1.5, $fn=32);
            translate([1.5, 2.6]) circle(r=1.5, $fn=32);
        }
    }
}

module usbc_slot_with_holder() {
	usbc_slot_inner_padding = 0.5;

    board_rest_width = 30; // board width slot
    board_rest_height = 2; // board height slot
	board_rest_depth = wall_thickness - usbc_port_overhang;
	corner_rest_offset = 4; // distance from edge of rest
	button_padding_height = usbc_slot_height;
	


    // Board Rest
    translate([
		0, 
		board_rest_height + usbc_slot_height/2, 
		usbc_port_overhang
	]) {
        cube([
			board_rest_width, 
			usbc_slot_height, 
			wall_thickness-usbc_port_overhang]);
    }


    // USB-C port hole
    translate([(board_rest_width - usbc_slot_width)/2, 0.6, 0]) {
        usbc_slot();
    }

	// Button padding right
	translate([
		corner_rest_offset, 
		0.6, 
		usbc_port_overhang]) {
		cube([board_rest_width-corner_rest_offset*2, button_padding_height, board_rest_depth]);
	}
}

function get_mount_screw_head_hole_pos_and_rot() = 
	[
		[screw_mount_width/2-0.5, wall_thickness, screw_mount_height/2],
		[90,90,0]
	];


// Plain screw hole, definable depth
module screw_hole(depth) {
	linear_extrude(height=depth) {
		circle(r=screw_hole_radius, $fn=20);
	}
}

// Screw head hole for outer wall (entrance to screw)
module screw_head_hole() {
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
module screw_mount_block(is_head_in_wall=true) {
	rounding_cutoff = 1;
	

	included_head_depth = is_head_in_wall ? screw_head_depth : 0;
	mount_hole_depth = screw_hole_depth - wall_thickness + included_head_depth;
	block_depth = mount_hole_depth + rounding_cutoff + mount_block_back_padding;
	
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
					screw_hole(mount_hole_depth);
				}
			}			
		}
	}
}

module temperature_sensor_hole() {
	// inside the case
	inside_layer_radius = 23 / 2; // 1.7 tolerance

	// outside the case (small hole)
	outside_layer_radius = 13.5 / 2; // 0.5 diam tolerance

	// Extra sturdy but close to edge
	small_hole_depth = 3;

	translate([0,small_hole_depth,0]) {
		rotate([270,0,0]) {
			linear_extrude(height=wall_thickness-small_hole_depth) {
				circle(r=inside_layer_radius, $fn=50);
			}


			translate([0,0,-small_hole_depth]) {
				linear_extrude(height=small_hole_depth) {
					circle(r=outside_layer_radius, $fn=50);
				}
			}
		}
	}
}

module ir_hole() {
	// inside the case
	inside_layer_radius = 5 / 2; // 1.7 tolerance

	// outside the case (small hole)
	outside_layer_radius = 3.9 / 2; // 0.5 diam tolerance

	// 1.2 padding for inner padding
	large_hole_depth = 1.5;

	translate([0,0,large_hole_depth]) {
		rotate([180,0,0]) {
			linear_extrude(height=large_hole_depth) {
				circle(r=inside_layer_radius, $fn=50);
			}


			translate([0,0,-wall_thickness+large_hole_depth]) {
				linear_extrude(height=wall_thickness-large_hole_depth) {
					circle(r=outside_layer_radius, $fn=50);
				}
			}
		}
	}
}

// Sticks out of the case
module mode_switch_hole() {
	width = 20.3;
	height = 8.5;
	translate([0,-width/2,-height/2]) {
		rotate([90,0,90]) {
			cube([width,height,wall_thickness]);
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



// test print pieces (test_slot_fitting_print.stl)
module test_print_pieces() {

	module _test_screw_head_hole(hole_diam) {
		translate([0, 0, wall_thickness-screw_head_depth]) {
			// Head hole
			linear_extrude(height=screw_head_depth) {
				circle(r=screw_head_radius, $fn=100);
			}

		}
		// wall hole

		linear_extrude(height=wall_thickness) {
			circle(r=hole_diam/2, $fn=20);
		}
	}


	difference() {
		// Board holder
		cube([30,40,wall_thickness]);	

		// Clip holder
		translate([5,35,0]) {
			rotate([0,0,270]) {
				usbc_slot_with_holder();
			}
		}


		// Clip slot
		rotate([180,0,90]) {
			backplate_screw_hole_holder(5, 20, -wall_thickness, false);
		}

		
		// Screw head holes

		translate([23,17,0]) {			
			translate([-5, 0, 3]) {
				test_text("2.0");
			}
			rotate([0,0,0]) {
				_test_screw_head_hole(2.0);
			}
		}

		translate([23,25,0]) {			
			translate([-5, 0, 3]) {
				test_text("2.1");
			}
			rotate([0,0,0]) {
				_test_screw_head_hole(2.1);
			}
		}

		translate([23,33,0]) {			
			translate([-5, 0, 3]) {
				test_text("2.2");
			}
			rotate([0,0,0]) {
				_test_screw_head_hole(2.2);
			}
		}

		
	}

	// second test board, clip + ir
	translate([70,-30,0]) {
		rotate([0,0,90]) {
			translate([40,0,0]) {
				difference() {
					cube([20,20,wall_thickness]);
					translate([10,8,wall_thickness]) {
						rotate([0,180,0]) {
							ir_hole();
						}
					}
				}
				
			}

			translate([0,0,0]) {
				rotate([0,0,180]) {
					backplate_screw_hole_holder(-55, -30, 0, true);
				}
			}
		}
	}

	
		


}

module test_text(text) {
	rotate([0,0,90]) {
		linear_extrude(height=1)
			text(text, size=2, font="Arial:style=Bold", halign="center");
	}
}
