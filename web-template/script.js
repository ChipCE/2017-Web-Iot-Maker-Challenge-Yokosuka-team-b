//global var
var reset=0;

//set navbar stay optop after scroll
$(window).scroll(function()
{
    if($(this).scrollTop()>=$('#headerImage').height())
    {
        $("#topNavBar").addClass("navbar-fixed-top");
        $("#headerImage").addClass("hide");
        $("#headerImage").removeClass("fill");
        $("#main-container").addClass("fixTopMargin");
        //$("#exTab3").addClass("fixTopMargin");
        if(reset==0)
        {
            $(window).scrollTop(0);
            reset=1;
        }
    }
});

var auto_refresh = setInterval(
function ()
{
$('#trainStat').load('trainStatus.php').fadeIn("slow");
}, 1000);

var auto_refresh_time = setInterval(
function ()
{
$('#cTime').load('timer.php').fadeIn("slow");
}, 1000);