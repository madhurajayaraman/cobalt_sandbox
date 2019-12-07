// Copyright 2019 The Cobalt Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef COBALT_UPDATER_UPDATER_MODULE_H_
#define COBALT_UPDATER_UPDATER_MODULE_H_

#include <memory>
#include <string>

#include "base/memory/scoped_refptr.h"
#include "base/message_loop/message_loop.h"
#include "cobalt/network/network_module.h"
#include "cobalt/updater/configurator.h"
#include "components/prefs/pref_service.h"
#include "components/update_client/crx_update_item.h"
#include "components/update_client/update_client.h"
#include "starboard/event.h"

namespace cobalt {
namespace updater {

// An interface that observes the updater. It provides notifications when the
// updater changes status.
class Observer : public update_client::UpdateClient::Observer {
 public:
  explicit Observer(scoped_refptr<update_client::UpdateClient> update_client)
      : update_client_(update_client) {}

  // Overrides for update_client::UpdateClient::Observer.
  void OnEvent(Events event, const std::string& id) override {
    update_client_->GetCrxUpdateState(id, &crx_update_item_);
  }

  const update_client::CrxUpdateItem& crx_update_item() const {
    return crx_update_item_;
  }

 private:
  scoped_refptr<update_client::UpdateClient> update_client_;
  update_client::CrxUpdateItem crx_update_item_;
  DISALLOW_COPY_AND_ASSIGN(Observer);
};

// UpdaterModule checks for available Cobalt update and downloads the update
// that matches the underlying Starboard ABI (SABI) configuration. Then
// the update is unpacked to a dedicated location for installation. The update
// checks run according to a schedule defined by the Cobalt application.
class UpdaterModule {
 public:
  UpdaterModule(base::MessageLoop* message_loop,
                network::NetworkModule* network_module);
  ~UpdaterModule();

  void MarkSuccessful();
  void Update();

 private:
  base::MessageLoop* message_loop_;
  scoped_refptr<update_client::UpdateClient> update_client_;
  std::unique_ptr<Observer> updater_observer_;
  network::NetworkModule* network_module_;
  scoped_refptr<Configurator> updater_configurator_;
};

}  // namespace updater
}  // namespace cobalt

#endif  // COBALT_UPDATER_UPDATER_MODULE_H_