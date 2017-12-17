#!/usr/bin/env python

from TuningTools.parsers import ArgumentParser, loggerParser
from RingerCore import emptyArgumentsPrintHelp

mainParser = ArgumentParser( description = 'Calculate relevance of a specific input variable',
                             add_help = False )
dataArgs = mainParser.add_argument_group( "Required arguments", "" )
dataArgs.add_argument( '-inO', '--originalFile', action='store',
                       metavar='o_file', required=True,
                       help="""The crossValStatAnalysis file of the discriminator trained
                               with ALL variables without modifications"""
                     )
dataArgs.add_argument( '-inM', '--modifiedFile', action='store',
                       metavar='m_file', required=True, nargs='+',
                       help="""The crossValStatAnalysis files of the discriminators 
                               trained without one of its variables (substituted by its 
                               mean according to Relevance normalization - in PreProc.py)
                               Obs.:
                                - One file per variable
                                - Files in order from the first variable (0) to the last 
                               N-1 (for N varia
                               bles)"""
                     )
dataArgs.add_argument( '-o', '--outputFileBase', action='store',
                       metavar='out', required=True,
                       help="""Base name for the output file"""
                     )

dataArgs.add_argument( '--label', action='store',
                       required=False, nargs='+',
                       help="""Name of the variables that are 
                               being analized."""
                     )


parser = ArgumentParser( description = 'Determine relevance of a discriminating variable.',
                         parents = [loggerParser, mainParser])
parser.make_adjustments()

emptyArgumentsPrintHelp( parser )

# Retrieve parser args:
args = parser.parse_args()


# Set up logger
from RingerCore import printArgs, Logger
logger = Logger.getModuleLogger( __name__, args.output_level )
printArgs( args, logger.debug )


# Calculate the dSP for each variable
from RingerCore import load
from math import sqrt

delta = []
meanDelta = []
stdDevDelta = []

nVar = len(args.modifiedFile)
logger.info('Total of %i variables analyzed.'%(nVar))
if args.label:
  if len(args.label) != nVar:
    logger.warning('Number of labels given do not match the number of variables. Unsetting labels usage.')
    args.label = None

# reference = {}
# for sort in args.o_file['infoPPChain'].keys():
#   reference[sort] = args.o_file['OperationPoint_Offline_LH_Medium_SP']['config_006'][sort]['infoOpBest']['sp']

logger.verbose('Reading original file...')
oFile = load(args.originalFile)

for var in range(len(args.modifiedFile)):
  logger.verbose('Reading modified file %i...'%(var))
  mFile = load(args.modifiedFile[var])
  tmp = []
  for s in range(len(oFile['infoPPChain'].keys())):
    tmp = tmp + \
        [ oFile['OperationPoint_Offline_LH_Medium_SP']['config_006']['sort_%1.3i'%(s)]['infoOpBest']['sp'] - \
        mFile['OperationPoint_Offline_LH_Medium_SP']['config_006']['sort_%1.3i'%(s)]['infoOpBest']['sp'] ]
    if args.label:
      logger.debug('Variable %s, Sort %i: delta SP value %f'%(args.label[var].upper(),s,tmp[s]))
    else:
      logger.debug('Variable %i, Sort %i: delta SP value %f'%(var,s,tmp[s]))
  delta = delta + [tmp]
  meanDelta = meanDelta + [ sum(tmp)/len(tmp) ]
  stdDevDelta = stdDevDelta + [ sqrt( sum( [ (tmp[i] - meanDelta[var])**2 for i in range(len(tmp)) ] )/len(tmp) ) ]
  if args.label:
    logger.info('Variable %s: delta SP value (%f +- %f) '%\
        (args.label[var].upper(), meanDelta[var], stdDevDelta[var]))
  else:
    logger.info('Variable %i: delta SP value (%f +- %f) '%(var, meanDelta[var], stdDevDelta[var]))


# Generate output file
# TODO: TXT file with all deltas, means and sDev


# Generate graphics
from ROOT import TCanvas, TGraphErrors, TImage
from ROOT import gROOT
from array import array

logger.verbose('Creating graphics...')
canvas = TCanvas( 'c', 'Relevance of training variables', 200, 10, 700, 500 )
canvas.SetGrid()
canvas.GetFrame().SetFillColor( 21 )
canvas.GetFrame().SetBorderSize( 12 )

n = len(meanDelta)
var = array( 'f', range(n) )
eVar = array( 'f', [0]*n )
delt = array( 'f', meanDelta*100 )
eDelt = array( 'f', stdDevDelta*100 )

gr = TGraphErrors( n, var, delt, eVar, eDelt )
gr.SetTitle( 'Relevance' )
gr.SetMarkerColor( 4 )
gr.SetMarkerStyle( 21 )
gr.Draw( 'ALP' )

canvas.Update()

fileName = "%s.png"%(args.outputFileBase)
img = TImage.Create()
img.FromPad(canvas)
logger.verbose("Saving file...")
img.WriteImage( fileName )
logger.info("Saved file '%s'"%(fileName))
