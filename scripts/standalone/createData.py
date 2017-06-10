#!/usr/bin/env python

from RingerCore import emptyArgumentsPrintHelp, ArgumentParser
from TuningTools.parsers import createDataParser, loggerParser

parser = ArgumentParser(description = 'Create TuningTool data from PhysVal.',
                        parents = [createDataParser, loggerParser])
parser.make_adjustments()

emptyArgumentsPrintHelp(parser)

# Retrieve parser args:
args = parser.parse_args( )
# Treat special argument
if len(args.reference) > 2:
  raise ValueError("--reference set to multiple values: %r", args.reference)
if len(args.reference) is 1:
  args.reference.append( args.reference[0] )
from RingerCore import Logger, LoggingLevel, printArgs, NotSet
logger = Logger.getModuleLogger( __name__, args.output_level )
<<<<<<< HEAD
# FIXME: Even if I am giving Offline as operation, args.operation!='Offline' is responding as True
if args.operation != 'Offline' and not args.treePath:
=======
if args.operation < 0 and not args.treePath:
>>>>>>> db8262242d91a257f9dc4718bbe247581c25b0d4
  ValueError("If operation is not set to Offline, it is needed to set the TreePath manually.")

printArgs( args, logger.debug )

crossVal = NotSet
if args.crossFile not in (None, NotSet):
  from TuningTools import CrossValidArchieve
  with CrossValidArchieve( args.crossFile ) as CVArchieve:
    crossVal = CVArchieve
  del CVArchieve

from TuningTools import createData
createData( args.sgnInputFiles, 
            args.bkgInputFiles,
            ringerOperation       = args.operation,
            referenceSgn          = args.reference[0],
            referenceBkg          = args.reference[1],
            treePath              = args.treePath,
            efficiencyTreePath    = args.efficiencyTreePath,
            pattern_oFile         = args.pattern_output_file,
            efficiency_oFile      = args.efficiency_output_file,
            l1EmClusCut           = args.l1EmClusCut,
            l2EtCut               = args.l2EtCut,
            efEtCut               = args.efEtCut,
            offEtCut              = args.offEtCut,
            level                 = args.output_level,
            nClusters             = args.nClusters,
            getRatesOnly          = args.getRatesOnly,
            etBins                = args.etBins,
            etaBins               = args.etaBins,
            ringConfig            = args.ringConfig,
            extractDet            = args.extractDet,
            standardCaloVariables = args.standardCaloVariables,
            useTRT                = args.useTRT,
            toMatlab              = args.toMatlab,
            plotMeans             = args.plotMeans,
            plotProfiles          = args.plotProfiles,
            label                 = args.label,
            crossVal              = crossVal,
            supportTriggers       = args.supportTriggers,
            pileupRef             = args.pileupRef,
          )

