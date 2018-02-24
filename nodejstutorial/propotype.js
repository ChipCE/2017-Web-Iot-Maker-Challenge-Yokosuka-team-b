function User()
{
    this.name = "";
    this.HP = 100;
    this.power = 3;

    function powerUp()
    {
        this.power++;
    }
}


//creater player from User obj
player = new User();
enemy = new User();

debugger;

//add function to User propotype
User.prototype.attack = function attack(target)
{
    target.HP = target.HP-this.power;
};

//make player att enemy
player.attack(enemy);

console.log("EnemyHP = ",enemy.HP);

//add exp point member
User.prototype.exp = 0;

console.log("Player Exp = ",player.exp);