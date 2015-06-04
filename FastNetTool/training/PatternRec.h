#ifndef FASTNETTOOL_TRAINING_PATTERNREC_H
#define FASTNETTOOL_TRAINING_PATTERNREC_H

#include <vector>
#include "FastNetTool/training/Training.h"

using namespace msg;

class PatternRecognition : public Training
{
protected:
  const REAL **inTrnList;
  const REAL **inValList;
  const REAL **inTstList;
  const REAL **targList;
  REAL **epochValOutputs;
  REAL **epochTstOutputs;
  unsigned numPatterns;
  unsigned inputSize;
  unsigned outputSize;
  ///If not mse criteria, I need enable the SP flag
  bool useSP;
  bool hasTstData;
  REAL bestGoalSP;
  REAL bestGoalDet;
  REAL bestGoalFa;
  REAL signalWeight;
  REAL noiseWeight;
  std::vector<DataManager*> dmTrn;
  unsigned *numValEvents;
  unsigned *numTstEvents;
  ///This will be used to select the validation criteria
  TrainGoal trainGoal;

  void allocateDataset(vector<DataHandler<REAL>*> dataSet, const bool forTrain, 
                        const REAL **&inList, REAL **&out, unsigned *&nEv);

  void deallocateDataset(const bool forTrain, const REAL **&inList, REAL **&out, unsigned *&nEv);
  
  void getNetworkErrors(const REAL **inList, const unsigned *nEvents, REAL **epochOutputs, REAL &mseRet, REAL &spRet, REAL &detRet, REAL &faDet);

  ///Helper function
  void isBestGoal( const REAL currError, REAL &bestGoalRet, ValResult &isBestRet ){
    if (currError > bestGoalRet)
    {
      bestGoalRet = currError;
      isBestRet = BETTER;
    }
    else if (currError < bestGoalRet) isBestRet = WORSE;
    else isBestRet = EQUAL;
  };



private:
    ///Name of the aplication
    string        m_appName;
    ///Hold the output level that can be: verbose, debug, info, warning or
    //fatal. This will be administrated by the MsgStream Class manager.
    Level         m_msgLevel;
    /// MsgStream for monitoring
    MsgStream     *m_log;



public:

  PatternRecognition(FastNet::Backpropagation *net, vector<DataHandler<REAL>*> inTrn, vector<DataHandler<REAL>*> inVal, 
                      vector<DataHandler<REAL>*> inTst, TrainGoal  mode, const unsigned bSize,
                      const REAL signalWeigh = 1.0, const REAL noiseWeight = 1.0, Level msglevel = INFO);

  virtual ~PatternRecognition();

  /// Calculates the SP product.
  /**
  Calculates the SP product. This method will run through the dynamic range of the outputs,
  calculating the SP product in each lambda value. Returning, at the end, the maximum SP
  product obtained.
  @return The maximum SP value obtained. You can hold the signal and noise effic pass by reference.
  */
  virtual REAL sp(const unsigned *nEvents, REAL **epochOutputs, REAL &det, REAL &fa );

  virtual void tstNetwork(REAL &mseTst, REAL &spTst, REAL &detTst, REAL &faTst)
  {
    MSG_DEBUG(m_log, "Starting testing process for an epoch.");
    getNetworkErrors(inTstList, numTstEvents, epochTstOutputs, mseTst, spTst, detTst, faTst);
  }


  /// Applies the validating set of each pattern for the network's validation.
  /**
  This method takes the one or more pattern's validating events (input and targets) and presents them
  to the network. At the end, the mean training error is returned. Since it is a validating function,
  the network is not modified, and no updating weights values are calculated. This method only
  presents the validating sets and calculates the mean validating error obtained.
  @param[in] net the network class that the events will be presented to. The internal parameters
  of this class are not modified inside this method, since it is only a network validating process.
  @return The mean validating error obtained after the entire training set is presented to the network.
  */
  virtual void valNetwork(REAL &mseVal, REAL &spVal, REAL &detVal, REAL &faVal)
  {
    MSG_DEBUG(m_log, "Starting validation process for an epoch.");
    getNetworkErrors(inValList, numValEvents, epochValOutputs, mseVal, spVal, detVal, faVal);
  }


  /// Applies the training set of each pattern for the network's training.
  /**
  This method takes the one or more patterns training events (input and targets) and presents them
  to the network, calculating the new mean (if batch training is being used) update values 
  after each input-output pair of each individual pattern is presented. At the end, the mean training error is returned.
  @param[in] net the network class that the events will be presented to. At the end,
  this class is modificated, as it will contain the mean values of \f$\Delta w\f$ and \f$\Delta b\f$ obtained
  after the entire training set has been presented, but the weights are not updated at the 
  end of this function. To actually update the weights, the user must call the proper
  class's method for that.
  @return The mean training error obtained after the entire training of each pattern set is presented to the network.
  */
  virtual REAL trainNetwork();

  virtual void showInfo(const unsigned nEpochs) const;

  virtual void isBestNetwork(const REAL currMSEError, const REAL currSPError, const REAL currDetError,
                             const REAL currFaError,  ValResult &isBestMSE, ValResult &isBestSP,
                             ValResult &isBestDet,    ValResult &isBestFa );
  

  virtual void showTrainingStatus(const unsigned epoch, const REAL mseTrn, const REAL mseVal, const REAL spVal, 
                                    const int stopsOn);
  

  virtual void showTrainingStatus(const unsigned epoch, const REAL mseTrn, const REAL mseVal, const REAL spVal, 
                                  const REAL mseTst, const REAL spTst, const int stopsOn);



 
  
};

#endif
