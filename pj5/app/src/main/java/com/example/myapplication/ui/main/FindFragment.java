package com.example.myapplication.ui.main;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.myapplication.Shop;
import com.example.myapplication.ShopManager;
import com.example.myapplication.databinding.FragmentFindBinding;

import androidx.fragment.app.Fragment;

/**
 *
 * determines the functionality in the FIND page,
 * uses hashtable to find whether the shop with given name exists
 * if the shop exists, print its name, rating and average cost
 * otherwise, print "None"
 *
 */
public class FindFragment extends Fragment {
    FragmentFindBinding binding;

    public FindFragment() {
        // Required empty public constructor
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        binding = FragmentFindBinding.inflate(getLayoutInflater());
        binding.findButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (binding.findTxt.getText().toString().length() == 0) {
                    binding.findResultTxt.setText("None");
                    return;
                }

                long ptr = ShopManager.getInstance().findByName(binding.findTxt.getText().toString());

                if (ptr > 0) {
                    binding.findResultTxt.setText(String.format("shop: %s,rating: %d,average cost: %s\n", Shop.getName(ptr), Shop.getRating(ptr), Shop.getAvgCost(ptr)));
                } else {
                    binding.findResultTxt.setText("None");
                }
            }
        });

        return binding.getRoot();
    }
}