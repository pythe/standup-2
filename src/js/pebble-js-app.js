var url;

var STORY_TYPE = {
  BUG: 0,
  CHORE: 1,
  FEATURE: 2,
  RELEASE: 3
}

var STORY_STATE = {
  unstarted: 0,
  started: 1,
  finished: 2,
  delivered: 3
}

Pebble.addEventListener('ready', function(e) {
  console.log("JS Up and running!");

  Pebble.addEventListener('appmessage', function(e) {
    console.log("Received message: ", JSON.stringify(e.payload));
  });

  Pebble.addEventListener('showConfiguration', function(e) {
    if (localStorage.getItem('apiKey')) {
      url = 'https://standup-config.cfapps.io/login?api_token=' + localStorage.getItem('apiKey');
    } else {
      url = 'https://standup-config.cfapps.io/login';
    }
    Pebble.openURL(url);
  });

  Pebble.addEventListener('webviewclosed', function(e) {
    var config_data = JSON.parse(decodeURIComponent(e.response));
    console.log('Config window returned: ', JSON.stringify(config_data));

    localStorage.setItem('projectIds', configData.project_ids);
    localStorage.setItem('apiKey', configData.api_token);
    localStorage.setItem('initials', configData.initials);
  });

  // Get set of stories using XMLHttpRequest
  var stories = requestMyStories();
  sendList(stories);
});

function sendItems(items, index, done) {
    Pebble.sendAppMessage(items[index], function() {
      if (index+1 < items.length) {
        sendItems(items, index+1, done);
      } else { done(); }
    }, function() {
      console.log('Item transmission failed: ' + index);
    });
};

function sendList(items) {
  Pebble.sendAppMessage({count: items.length});
  sendItems(items, 0, function() { Pebble.sendAppMessage({complete: 1}) });
};

function requestMyStories() {
  // soon, use XMLHttpRequest to create this shape of object
  return [
    {id: 1, type: STORY_TYPE.BUG, state: STORY_STATE.delivered, name: "The story name can be rather long so perhaps it should be ellipsified"},
    {id: 2, type: STORY_TYPE.FEATURE, state: STORY_STATE.started, name: "The story name can be rather long so perhaps it should be ellipsified"},
    {id: 3, type: STORY_TYPE.CHORE, state: STORY_STATE.finished, name: "The story name can be rather long so perhaps it should be ellipsified"},
    {id: 4, type: STORY_TYPE.RELEASE, state: STORY_STATE.unstarted, name: "The story name can be rather long so perhaps it should be ellipsified"}
  ];
}
//
//
//
// Settings.config(
//   {url: url},
//   function() {},
//   function(e) {
//     console.log("configuration changed:");
//     console.log(JSON.stringify(e));
//     if (e.options.clear) {
//       Settings.option('project_ids', undefined);
//       Settings.option('api_token', undefined);
//       Settings.option('initials', undefined);
//     }
//     Standup.reset();
//     Standup.main();
//   });
//
// function request(options) {
//   return new Promise(function(resolve, reject) {
//     ajax(options, resolve, reject);
//   });
// }
//
// function flatten(arrayOfArrays) {
//   return arrayOfArrays.reduce(function(acc, val) {
//     return acc.concat(val);
//   }, []);
// }
//
// function groupBy(array, func) {
//   var grouped = {},
//     i;
//   for (i=0; i < array.length; i++) {
//     var element = array[i];
//     var value = func(element);
//     if (!(value in grouped))
//       grouped[value] = [];
//
//     grouped[value].push(element);
//   }
//
//   return grouped;
// }
//
// function capitalize(str) {
//   return str[0].toUpperCase() + str.substr(1);
// }
//
// var Standup = {
//   main: function() {
//     this.attributes.projectIds = Settings.option('project_ids');
//     this.attributes.apiKey = Settings.option('api_token');
//     this.attributes.initials = Settings.option('initials');
//     var configMessage = this.attributes.projectIds ? '' : 'Please open settings';
//     this.loadingCard = new UI.Card({
//       banner: "images/logo_splash.png",
//       body: configMessage
//     });
//
//     this.loadingCard.show();
//
//     this.fetch();
//   },
//   attributes: {
//   },
//   iconForStoryType: function(type) {
//     switch(type) {
//       case "feature":
//         return "images/feature.png";
//       case "bug":
//         return "images/bug.png";
//       case "chore":
//         return "images/chore.png";
//       case "release":
//         return "images/release.png";
//       default:
//         return "images/feature.png";
//     }
//   },
//   buildMenu: function(stories) {
//     stories.forEach(function(s) {
//       s.title = s.name;
//       s.subtitle = capitalize(s.Story_Type);
//     });
//
//     var grouped = groupBy(stories, function(story) {
//       return capitalize(story.current_state);
//     });
//
//     var sectionOrder = ["Started", "Finished", "Delivered", "Accepted"],
//         sections = [];
//
//     sectionOrder.forEach(function(sectionName){
//       if (grouped[sectionName]) {
//         var stories = grouped[sectionName];
//         sections.push({title: sectionName, items: stories});
//       }
//     });
//
//
//
//     return {
//       sections: sections
//     };
//   },
//   fetch: function() {
//     var self = this, initials = this.attributes.initials;
//     var promises = this.attributes.projectIds.map(function(projectId) {
//       var url = 'https://www.pivotaltracker.com/services/v5/projects/' + projectId + '/search?query=mywork:' + initials;
//       console.log("url ", url);
//       return request({
//         url: url,
//         type: 'json',
//         headers: {
//           "X-TrackerToken": self.attributes.apiKey
//         }
//       });
//     });
//     Promise.all(promises).then(function(resolutions) {
//       var myStories = flatten(resolutions.map(function(data) {
//         return data.stories.stories;
//       }));
//       self.loadingCard.hide();
//       if (myStories.length === 0) {
//         self.activeCard = new UI.Card({
//           title: 'No Stories',
//           subtitle: 'Get to work!'
//         });
//         self.activeCard.show();
//
//         return;
//       }
//
//       var menuItems = self.buildMenu.call(self, myStories);
//       var menu = new UI.Menu(menuItems);
//       menu.on('select', function(e) {
//         console.log("Using image ", self.iconForStoryType(e.item.Story_Type));
//         var card = new UI.Card({
//           subtitle: " ",
//           subicon: self.iconForStoryType(e.item.Story_Type),
//           body: e.item.name,
//           scrollable: true
//         });
//         self.activeCard = card;
//         card.show();
//       });
//       self.menu = menu;
//       menu.show();
//     }).catch(function(error) {
//       console.log("error! ", error);
//       console.log(JSON.stringify(error));
//     });
//     Promise.all(promises).catch(function() {
//       console.log("Fetch failed for some reason");
//     });
//   },
//
//   reset: function() {
//     if (this.menu) {
//       this.menu.hide();
//       this.menu = undefined;
//     }
//     if (this.activeCard) {
//       this.activeCard.hide();
//       this.activeCard = undefined;
//     }
//     if (this.loadingCard) {
//       this.loadingCard.hide();
//       this.loadingCard = undefined;
//     }
//   }
// };
//
//
// Standup.main();
