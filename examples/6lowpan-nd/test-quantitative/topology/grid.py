
outputfolder = 'topology/grid'
template = 'coojagen/templates/cooja-template-6lx.csc'
radio_model = 'udgm'
tx_range = 45
tx_interference = 45
topology = 'grid_ratio'
ratio = "1:1"
step = 35
mote_count = range(4,26,1)
assignment = {'all':'6lr', '0':'6lbr', '-1':'6lh'} 
multi_br=0
script_file = "[CONFIG_DIR]/../grid_script.js"

mote_types = []

mote_type_6lbr = {	'shortname':'6lbr', 
			'fw_folder':'[CONTIKI_DIR]/examples/6lowpan-nd/6lbr/', 
			'maketarget':'ex-6lowpannd-6lbr', 
			'makeargs':'WITH_STATISTICS=1', 
			'description':"6LBR manual routing",
			'serial':'socket' }

mote_type_6lr = {	'shortname':'6lr', 
			'fw_folder':'[CONTIKI_DIR]/examples/6lowpan-nd/6lr/', 
			'maketarget':'ex-6lowpannd-6lr', 
			'makeargs':'WITH_STATISTICS=1', 
			'description':"6LR manual routing",
			'serial':'socket' }

mote_type_6lh = {	'shortname':'6lh', 
			'fw_folder':'[CONTIKI_DIR]/examples/6lowpan-nd/6lh/', 
			'maketarget':'ex-6lowpannd-6lh', 
			'makeargs':'WITH_STATISTICS=1', 
			'description':"6LH manual routing",
			'serial':'socket' }


mote_types.append(mote_type_6lbr)
mote_types.append(mote_type_6lr)
mote_types.append(mote_type_6lh)
