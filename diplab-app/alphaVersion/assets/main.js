  (() => {
    const state = {};

    let context = null;
    let nodesToDestroy = [];
    let pendingUpdate = false;

    function destroyAnyNodes() {
      // destroy current view template refs before rendering again
      nodesToDestroy.forEach((el) => el.remove());
      nodesToDestroy = [];
    }

    // Function to update data bindings and loops
    // call update() when you mutate state and need the updates to reflect
    // in the dom
    function update() {
      if (pendingUpdate === true) {
        return;
      }
      pendingUpdate = true;

      document.querySelectorAll("[data-el='button']").forEach((el) => {
        el.setAttribute("openLinkInNewTab", false);
      });

      destroyAnyNodes();

      pendingUpdate = false;
    }

    // Update with initial state on first load
    update();
  })();