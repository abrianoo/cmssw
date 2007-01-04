#ifndef RecoMuon_GlobalTrackFinder_GlobalMuonTrajectoryBuilder_H
#define RecoMuon_GlobalTrackFinder_GlobalMuonTrajectoryBuilder_H

/** \class GlobalMuonTrajectoryBuilder
 *  class to build muon trajectory
 *
 *  $Date: 2007/01/03 20:37:45 $
 *  $Revision: 1.44 $
 *
 *  \author N. Neumeister 	 Purdue University
 *  \author C. Liu 		 Purdue University
 *  \author A. Everett 		 Purdue University
 */

#include "FWCore/Framework/interface/Handle.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoMuon/TrackingTools/interface/MuonTrajectoryBuilder.h"
#include "RecoMuon/TrackingTools/interface/RecoMuonEnumerators.h"
#include "RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h"
#include "RecoTracker/CkfPattern/interface/TrackerTrajectoryBuilder.h"

namespace edm {class Event;}

class RectangularEtaPhiTrackingRegion;
class TrajectoryStateOnSurface;

class GlobalMuonTrackMatcher;
class TransientTrackingRecHitBuilder;
class MuonTransientTrackingRecHitBuilder;

class MuonDetLayerMeasurements;
class MuonTrackReFitter;
class MuonTrackConverter;
class MuonServiceProxy;
class Trajectory;
class TrackerSeedGenerator;
class TrajectoryCleaner;
class MuonTrackLoader;

class GlobalMuonMonitorInterface;

//class CkfTrajectoryBuilder;


namespace edm {class ParameterSet; class Event; class EventSetup;}

class GlobalMuonTrajectoryBuilder : public MuonTrajectoryBuilder {

  public:

    typedef TransientTrackingRecHit::RecHitContainer RecHitContainer;
    typedef TransientTrackingRecHit::ConstRecHitContainer ConstRecHitContainer;
    typedef TransientTrackingRecHit::RecHitPointer RecHitPointer;
    typedef TransientTrackingRecHit::ConstRecHitPointer ConstRecHitPointer;

    typedef MuonTransientTrackingRecHit::MuonRecHitPointer MuonRecHitPointer;
    typedef MuonTransientTrackingRecHit::ConstMuonRecHitPointer ConstMuonRecHitPointer;
    typedef MuonTransientTrackingRecHit::MuonRecHitContainer MuonRecHitContainer;
    typedef MuonTransientTrackingRecHit::ConstMuonRecHitContainer ConstMuonRecHitContainer;

    typedef std::vector<Trajectory> TC;
    typedef TC::const_iterator TI;

  public:

    /// constructor with Parameter Set and MuonServiceProxy
    GlobalMuonTrajectoryBuilder(const edm::ParameterSet&, const MuonServiceProxy*, MuonTrackLoader*);
          
    /// destructor
    ~GlobalMuonTrajectoryBuilder();

    /// reconstruct trajectories from standalone and tracker only Tracks    
    MuonTrajectoryBuilder::CandidateContainer trajectories(const TrackCand&);

    /// dummy implementation, unused in this class
    MuonTrajectoryBuilder::TrajectoryContainer trajectories(const TrajectorySeed&) { return MuonTrajectoryBuilder::TrajectoryContainer(); }

    /// pass the Event to the algo at each event
    virtual void setEvent( edm::Event& );

  private:
    
    /// choose tracker tracks within region of interest
    std::vector<TrackCand> chooseRegionalTrackerTracks(const TrackCand&, 
                                                       const std::vector<TrackCand>&) const;

    /// define region of interest with tracker
    RectangularEtaPhiTrackingRegion defineRegionOfInterest(const reco::TrackRef&) const;

    /// build combined trajectory from sta Track and tracker RecHits
    MuonTrajectoryBuilder::CandidateContainer build(const TrackCand&, 
                                                    const std::vector<TrackCand>&) const;
  
    /// check muon RecHits, calculate chamber occupancy and select hits to be used in the final fit
    void checkMuonHits(const reco::Track&, ConstRecHitContainer&, 
                       ConstRecHitContainer&, 
                       std::vector<int>&) const;
 
    /// select muon hits compatible with trajectory; check hits in chambers with showers
    ConstRecHitContainer selectMuonHits(const Trajectory&, 
                                        const std::vector<int>&) const;
 
    /// choose final trajectory
    const Trajectory* chooseTrajectory(const std::vector<Trajectory*>&) const;

    /// calculate chi2 probability (-ln(P))
    double trackProbability(const Trajectory&) const;    

    /// print all RecHits of a trajectory
    void printHits(const ConstRecHitContainer&) const;

    /// build a tracker Trajectory from a seed
    TC makeTrajsFromSeeds(const std::vector<TrajectorySeed>&) const;

    /// make a TrackCand collection using tracker Track, Trajectory information
    std::vector<TrackCand> makeTkCandCollection(const TrackCand&) const;

    /// if TrackCand has only a TrackRef, attempt to add Trajectory*
    void addTraj(TrackCand&) const;

  private:

    GlobalPoint theVertexPos;
    GlobalError theVertexErr;
    GlobalMuonTrackMatcher* theTrackMatcher;
    MuonTrackReFitter* theRefitter;
    MuonDetLayerMeasurements* theLayerMeasurements;
    MuonTrackConverter* theTrackConverter;
    TrackerSeedGenerator* theTkSeedGenerator;
    TrajectoryCleaner* theTrajectoryCleaner;
    
    int   theMuonHitsOption;
    ReconstructionDirection theDirection;
    float thePtCut;
    float theProbCut;
    int   theHitThreshold;
    float theDTChi2Cut;
    float theCSCChi2Cut;
    float theRPCChi2Cut;
    edm::InputTag theTkTrackLabel;
    std::string theCkfBuilderName;
    std::string theL2SeededTkLabel;

    bool theTkTrajsAvailableFlag;
    bool theMakeTkSeedFlag;
    bool theFirstEvent;
    bool theMIMFlag;

    edm::ESHandle<TrackerTrajectoryBuilder> theCkfBuilder;
    edm::Handle<reco::TrackCollection> allTrackerTracks;

    const std::vector<Trajectory>* allTrackerTrajs;
 
    edm::Event* theEvent;

    const MuonServiceProxy *theService;
    MuonTrackLoader *theTrackLoader;

    GlobalMuonMonitorInterface* dataMonitor;

    
};
#endif
