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

// Fail if the deep link is not received within 15 seconds.
var kTimeout = 15 * 1000;
var failTimer = setTimeout(fail, kTimeout);

function fail() {
    console.log("Failing due to timeout!");
    assertTrue(false);
}

// The test sends "link 1", "link 2", "link 3" before load & so only "link 3"
// should be handled.
function listener(link) {
    console.log("Received link: " + link.toString());
    assertEqual("link 3", link);
    console.log("Ending test");
    onEndTest();
    clearTimeout(failTimer);
}

h5vcc.runtime.onDeepLink.addListener(listener);
console.log("Listener added");