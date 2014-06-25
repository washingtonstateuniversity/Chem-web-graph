      jQuery(document).ready(function() {

          $('#myshowhide').on("click",function(){
            $('#graphbox').toggle("slow");
          }) //end of my show/hide

          // first disable the graph editor and then send data to server
          $('#mysend').on("click",function(){
            $('#graphbox').block({message: '<h3>submitting</h3>'});

          })//end of mysend 

          $('#myreflesh').on("click",function(){
            $('#graphbox').unblock();
          })//end of myreflesh
      });