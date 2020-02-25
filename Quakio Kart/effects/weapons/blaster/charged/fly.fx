effect effects/weapons/blaster/charged/fly
{
	size	54

	spawner "line"
	{
		detail		0.5
		count		1,1
		locked
		constant

		line
		{
			duration	20,20
			material	"gfx/effects/weapons/blaster_fly"

			start
			{
				size { point 8 }
				offset { point 12,0,0 }
				length { point -25,0,0 }
			}
			end
			{
				size { point 2 }
				length { point -20,0,0 }
			}
		}
	}
}





