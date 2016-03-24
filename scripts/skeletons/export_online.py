#!/usr/bin/env python

from RingerCore.Logger import LoggingLevel
from RingerCore.FileIO import expandFolders
from TuningTools.CrossValidStat import CrossValidStatAnalysis
from TuningTools.FilterEvents import RingerOperation
from pprint import pprint

crossValGrid = expandFolders('$ROOTCOREBIN/../TuningTools/scripts/skeletons/','FixET_Norm1*.pic')

#configList = [
#                # EFCalo, Et 0, Eta 0
#                #  Pd, SP, Pf
#              [[   7,   7,  7   ],
#               [# Et 0, Eta 1
#                   16,  16,  7  ],
#               [# Et 0, Eta 2
#                   7,  19,  6   ],
#               [# Et 0, Eta 3
#                  5,   11, 14   ]],
#                # EFCalo, Et 1, Eta 0
#                #  Pd, SP, Pf
#              [[   9,  10, 7    ],
#               [# Et 1, Eta 1
#                   8,  16, 7    ],
#               [# Et 1, Eta 2
#                   11, 11, 6    ],
#               [# Et 1, Eta 3
#                   8,  6,  6    ]],
#                # EFCalo, Et 2, Eta 0
#                #  Pd, SP, Pf
#              [[   5,  14, 14   ],
#               [# Et 2, Eta 1
#                   11, 11, 11   ],
#               [# Et 2, Eta 2
#                   8,  9,  13   ],
#               [# Et 2, Eta 3
#                   15, 15, 20   ]],
#                # EFCalo, Et 3, Eta 0
#                #  Pd, SP, Pf
#              [[   10, 8,  16   ],
#               [# Et 3, Eta 1
#                   17, 7,  7    ],
#               [# Et 3, Eta 2
#                   8,  17, 15   ],
#               [# Et 3, Eta 3
#                  16,  16,  5  ]]
#            ]
#
#refBenchmarkList = [["Medium_LH_EFCalo_Pd","Medium_MaxSP","Medium_LH_EFCalo_Pf"]]
#
#chainNames=['e24_lhmedium_L1EM20VH_L2EFCalo_ringer_pd',
#            'e24_lhmedium_L1EM20VH_ringer_sp',
#            'e24_lhmedium_L1EM20VH_L2EFCalo_ringer_pf']

configList = [
                # EFCalo, Et 0, Eta 0
              [[ 7   ],
               [# Et 0, Eta 1
                 7  ],
               [# Et 0, Eta 2
                 19 ],
               [# Et 0, Eta 3
                 5 ]],
                # EFCalo, Et 1, Eta 0
                #  Pd, SP, Pf
              [[  7    ],
               [# Et 1, Eta 1
                  7    ],
               [# Et 1, Eta 2
                  11],
               [# Et 1, Eta 3
                  6    ]],
                # EFCalo, Et 2, Eta 0
                #  Pd, SP, Pf
              [[  14   ],
               [# Et 2, Eta 1
                  11],
               [# Et 2, Eta 2
                   8   ],
               [# Et 2, Eta 3
                   15  ]],
                # EFCalo, Et 3, Eta 0
                #  Pd, SP, Pf
              [[   16   ],
               [# Et 3, Eta 1
                   7    ],
               [# Et 3, Eta 2
                   8    ],
               [# Et 3, Eta 3
                  5  ]]
            ]


refBenchmarkList = [[ # Et 0, Eta 0
                    ["Medium_LH_EFCalo_Pf"],
                      # Et 0, Eta 1
                    ["Medium_LH_EFCalo_Pf"],
                      # Et 0, Eta 2
                    ["Medium_MaxSP"],
                      # Et 0, Eta 3
                    ["Medium_LH_EFCalo_Pd"]],
                      # Et 1, Eta 0
                    [["Medium_LH_EFCalo_Pf"],
                      # Et 1, Eta 1
                    ["Medium_LH_EFCalo_Pf"],
                      # Et 1, Eta 2
                    ["Medium_LH_EFCalo_Pd"],
                      # Et 1, Eta 3
                    ["Medium_LH_EFCalo_Pf"]],
                      # Et 2, Eta 0
                    [["Medium_LH_EFCalo_Pf"],
                      # Et 2, Eta 1
                    ["Medium_LH_EFCalo_Pf"],
                      # Et 2, Eta 2
                    ["Medium_LH_EFCalo_Pd"],
                      # Et 2, Eta 3
                    ["Medium_LH_EFCalo_Pd"]],
                      # Et 3, Eta 0
                    [["Medium_LH_EFCalo_Pf"],
                      # Et 3, Eta 1
                    ["Medium_LH_EFCalo_Pf"],
                      # Et 3, Eta 2
                    ["Medium_LH_EFCalo_Pd"],
                      # Et 3, Eta 3
                    ["Medium_LH_EFCalo_Pf"]],
                    ]


chainNames=['e24_lhmedium_EFCalo']

CrossValidStatAnalysis.exportDiscrFiles(crossValGrid,
                                        RingerOperation.L2,
                                        refBenchCol=refBenchmarkList,
                                        configCol=configList,
                                        triggerChains=chainNames
                                        )


