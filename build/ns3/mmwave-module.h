
#ifdef NS3_MODULE_COMPILATION
# error "Do not include ns3 module aggregator headers from other modules; these are meant only for end user scripts."
#endif

#ifndef NS3_MODULE_MMWAVE
    

// Module headers:
#include "core-network-stats-calculator.h"
#include "mc-stats-calculator.h"
#include "mc-ue-net-device.h"
#include "mmwave-amc.h"
#include "mmwave-beamforming-model.h"
#include "mmwave-bearer-stats-calculator.h"
#include "mmwave-bearer-stats-connector.h"
#include "mmwave-chunk-processor.h"
#include "mmwave-component-carrier-enb.h"
#include "mmwave-component-carrier-ue.h"
#include "mmwave-component-carrier.h"
#include "mmwave-control-messages.h"
#include "mmwave-eesm-cc-t1.h"
#include "mmwave-eesm-cc-t2.h"
#include "mmwave-eesm-cc.h"
#include "mmwave-eesm-error-model.h"
#include "mmwave-eesm-ir-t1.h"
#include "mmwave-eesm-ir-t2.h"
#include "mmwave-eesm-ir.h"
#include "mmwave-eesm-t1.h"
#include "mmwave-eesm-t2.h"
#include "mmwave-enb-mac.h"
#include "mmwave-enb-net-device.h"
#include "mmwave-enb-phy.h"
#include "mmwave-error-model.h"
#include "mmwave-flex-tti-mac-scheduler.h"
#include "mmwave-flex-tti-maxrate-mac-scheduler.h"
#include "mmwave-flex-tti-maxweight-mac-scheduler.h"
#include "mmwave-flex-tti-pf-mac-scheduler.h"
#include "mmwave-harq-phy.h"
#include "mmwave-helper.h"
#include "mmwave-interference.h"
#include "mmwave-lte-mi-error-model.h"
#include "mmwave-lte-rrc-protocol-real.h"
#include "mmwave-mac-csched-sap.h"
#include "mmwave-mac-pdu-header.h"
#include "mmwave-mac-pdu-tag.h"
#include "mmwave-mac-sched-sap.h"
#include "mmwave-mac-scheduler.h"
#include "mmwave-mac-trace.h"
#include "mmwave-mac.h"
#include "mmwave-net-device.h"
#include "mmwave-no-op-component-carrier-manager.h"
#include "mmwave-phy-mac-common.h"
#include "mmwave-phy-sap.h"
#include "mmwave-phy-trace.h"
#include "mmwave-phy.h"
#include "mmwave-point-to-point-epc-helper.h"
#include "mmwave-propagation-loss-model.h"
#include "mmwave-radio-bearer-tag.h"
#include "mmwave-rrc-protocol-ideal.h"
#include "mmwave-spectrum-phy.h"
#include "mmwave-spectrum-signal-parameters.h"
#include "mmwave-spectrum-value-helper.h"
#include "mmwave-ue-mac.h"
#include "mmwave-ue-net-device.h"
#include "mmwave-ue-phy.h"
#endif